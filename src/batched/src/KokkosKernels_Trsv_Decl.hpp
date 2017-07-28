#ifndef __KOKKOSKERNELS_TRSV_DECL_HPP__
#define __KOKKOSKERNELS_TRSV_DECL_HPP__


/// \author Kyungjoo Kim (kyukim@sandia.gov)

namespace KokkosKernels {
  namespace Batched {
    namespace Experimental {  
      ///
      /// Serial Trsv
      ///

      namespace Serial {
        template<typename ArgUplo,
                 typename ArgTrans,
                 typename ArgDiag,
                 typename ArgAlgo>
        struct Trsv {

          template<typename ScalarType,
                   typename AViewType,
                   typename bViewType>
          KOKKOS_INLINE_FUNCTION
          static int
          invoke(const ScalarType alpha,
                 const AViewType &A,
                 const bViewType &b);
        };
      }

      ///
      /// Team Trsv
      ///

      namespace Team {
        template<typename MemberType, 
                 typename ArgUplo,
                 typename ArgTrans,
                 typename ArgDiag,
                 typename ArgAlgo>
        struct Trsv {

          template<typename ScalarType,
                   typename AViewType,
                   typename bViewType>
          KOKKOS_INLINE_FUNCTION
          static int
          invoke(const MemberType &member,
                 const ScalarType alpha,
                 const AViewType &A,
                 const bViewType &b);
        };
      }

    }
  }
}
#endif