#ifndef __KOKKOSKERNELS_GEMM_SERIAL_INTERNAL_HPP__
#define __KOKKOSKERNELS_GEMM_SERIAL_INTERNAL_HPP__


/// \author Kyungjoo Kim (kyukim@sandia.gov)

#include "KokkosKernels_Util.hpp"

//#include "KokkosKernels_InnerGemmFixA_Serial_Impl.hpp"
//#include "KokkosKernels_InnerGemmFixB_Serial_Impl.hpp"
#include "KokkosKernels_InnerGemmFixC_Serial_Impl.hpp"

namespace KokkosKernels {

  ///
  /// Serial Internal Impl
  /// ==================== 
  namespace Serial {
    template<typename ArgAlgo>
    struct GemmInternal {
      template<typename ScalarType,
               typename ValueType>
      KOKKOS_INLINE_FUNCTION
      static int
      invoke(const int m, const int n, const int k,
             const ScalarType alpha, 
             const ValueType *__restrict__ A, const int as0, const int as1,
             const ValueType *__restrict__ B, const int bs0, const int bs1,
             const ScalarType beta,
             /**/  ValueType *__restrict__ C, const int cs0, const int cs1) {
        //static_assert("KokkosKernels::GemmInternal:: Not yet implemented");
        return 0;
      }
    };

    template<>
    template<typename ScalarType,
             typename ValueType>
    KOKKOS_INLINE_FUNCTION
    int
    GemmInternal<Algo::Gemm::Unblocked>::
    invoke(const int m, const int n, const int k,
           const ScalarType alpha, 
           const ValueType *__restrict__ A, const int as0, const int as1,
           const ValueType *__restrict__ B, const int bs0, const int bs1,
           const ScalarType beta,
           /**/  ValueType *__restrict__ C, const int cs0, const int cs1) {
      // C = beta C + alpha A B
      // C (m x n), A(m x k), B(k x n)
      
      typedef ValueType value_type;
      
      if      (beta == 0) Util::set  (m, n, C, cs0, cs1, value_type(0)   );
      else if (beta != 1) Util::scale(m, n, C, cs0, cs1, value_type(beta));
      
      if (alpha != 0) {
        if (m <= 0 || n <= 0 || k <= 0) return 0;
        
        value_type
          *__restrict__ pC = C;
        for (int p=0;p<k;++p) {
          const value_type
            *__restrict__ pA = A+p*as1,
            *__restrict__ pB = B+p*bs0;
          for (int i=0;i<m;++i) {
            const value_type tA(alpha*pA[i*as0]);
            for (int j=0;j<n;++j)
              pC[i*cs0+j*cs1] += tA*pB[j*bs1];
          }
        }
      }
      return 0;
    }

    template<>
    template<typename ScalarType,
             typename ValueType>
    KOKKOS_INLINE_FUNCTION
    int
    GemmInternal<Algo::Gemm::Blocked>::
    invoke(const int m, const int n, const int k,
           const ScalarType alpha, 
           const ValueType *__restrict__ A, const int as0, const int as1,
           const ValueType *__restrict__ B, const int bs0, const int bs1,
           const ScalarType beta,
           /**/  ValueType *__restrict__ C, const int cs0, const int cs1) {
      // C = beta C + alpha A B
      // C (m x n), A(m x k), B(k x n)

      typedef ValueType value_type;      

      if      (beta == 0) Util::set  (m, n, C, cs0, cs1, value_type(0)   );
      else if (beta != 1) Util::scale(m, n, C, cs0, cs1, value_type(beta));
      
      if (alpha != 0) {
        if (m <= 0 || n <= 0 || k <= 0) return 0;

        // small matrix specialization
        {
          enum : int {
            mb = Algo::Gemm::Blocked::mb,
            nb = Algo::Gemm::Blocked::nb };
          
          InnerGemmFixC<> inner(as0, as1, bs0, bs1, cs0, cs1);
          
          auto gemm = [&](const int ib, 
                          const int jb,
                          const int pb,
                          const value_type *__restrict__ AA,
                          const value_type *__restrict__ BB,
                          /**/  value_type *__restrict__ CC) {
            for (int i=0;i<ib;i+=mb) 
              for (int j=0;j<jb;j+=nb)
                inner.serial_invoke(alpha, 
                                    AA+i*as0, BB+j*bs1, 
                                    (i+mb) > ib ? (ib-i) : mb, 
                                    (j+nb) > jb ? (jb-j) : nb, 
                                    pb, 
                                    CC+i*cs0+j*cs1);
          };          

          const bool is_small = (m*n*k <= 64*64*64);
          if (is_small) {
            gemm(m, n, k, A, B, C);
          } else {
            // cache blocking
            const int 
              nc = nb*10, kc = mb*4, mc = mb*4;
            
            for (int jj=0;jj<n;jj+=nc) {
              const int tj = n-jj, jb = (tj < nc ? tj : nc);
              for (int pp=0;pp<k;pp+=kc) {
                const int tp = k-pp, pb = (tp < kc ? tp : kc);
                //const int pb = k, pp = 0;
                for (int ii=0;ii<m;ii+=mc) {
                  const int ti = m-ii, ib = (ti < mc ? ti : mc);
                  
                  const value_type *__restrict__ AA = A+ii*as0+pp*as1;
                  const value_type *__restrict__ BB = B+pp*bs0+jj*bs1;
                  /**/  value_type *__restrict__ CC = C+ii*cs0+jj*cs1;
                  
                  gemm(ib, jb, pb, AA, BB, CC);                  
                } // for ii
              } // for pp
            } // for jj
          }
        }
      }
      return 0;
    }
    
  } // end namespace Serial

} // end namespace KokkosKernels

#endif