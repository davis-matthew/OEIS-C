/*
 * A383756  Expansion of 1/Product_{k=0..2} (1 - 3^k * 4^(2-k) * x).
 * https://oeis.org/A383756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37*a(n-1) - 444*a(n-2) + 1728*a(n-3) Initial values:
 *            a(0)=1, a(1)=37, a(2)=925
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>

/* Helper to perform multiplication with 128‑bit intermediate for integral types */
template<typename T, bool Integral = std::is_integral<T>::value>
struct mul_impl {
    static inline seq_t mul(seq_t x, unsigned int c) { return x * c; }
};

template<typename T>
struct mul_impl<T, true> {
    static inline seq_t mul(seq_t x, unsigned int c) {
#if defined(__SIZEOF_INT128__)
        return (seq_t)(((unsigned __int128)x) * c);
#else
        return x * c;
#endif
    }
};

#define MUL_CONST(x,c) (mul_impl<seq_t>::mul((x),(c)))
#else
/* C compilation: use 128‑bit cast if supported */
#if defined(__SIZEOF_INT128__)
#define MUL_CONST(x,c) ( (seq_t)( (unsigned __int128)(x) * (c) ) )
#else
#define MUL_CONST(x,c) ( (x) * (c) )
#endif
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 37;
    if (n == 2) return 925;

    seq_t prev3 = 1;   /* a(n-3) */
    seq_t prev2 = 37;  /* a(n-2) */
    seq_t prev1 = 925; /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t t1 = MUL_CONST(prev1, 37);
        seq_t t2 = MUL_CONST(prev2, 444);
        seq_t t3 = MUL_CONST(prev3, 1728);
        /* All intermediate results fit in the intermediate type, final result fits in seq_t */
        cur = t1 - t2 + t3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
