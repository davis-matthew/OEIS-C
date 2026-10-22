/*
 * A359646  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(5*n+k,k).
 * https://oeis.org/A359646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(5*n + k, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for any seq_t/idx_t combination supported by the build. */

#ifdef __cplusplus
#include <type_traits>

/* Helper to perform multiplication in 128‑bit when seq_t is an integral type. */
template <typename T>
static inline T mul128(T a, T b) {
    if constexpr (std::is_integral_v<T>) {
        unsigned __int128 temp = (unsigned __int128)a * (unsigned __int128)b;
        return (T)temp;
    } else {
        return a * b;   /* seq_t is a big‑integer type. */
    }
}
#endif

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    seq_t sum   = 0;
    seq_t term  = 1;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        sum += term;
        if (i == n) break;            /* avoid an unnecessary iteration after the last sum */

#ifdef __cplusplus
        /* Use 128‑bit temp for intermediate calculations when seq_t is integral. */
        seq_t a1 = (seq_t)(n - i);
        seq_t a2 = (seq_t)(5 * n + i + 1);
        seq_t b  = (seq_t)(i + 1);

        seq_t temp = mul128(term, a1);
        temp = mul128(temp, a2);
        temp = temp / b;
        temp = temp / b;
        term = temp;
#else
        /* For pure C, cast to unsigned __int128 for safe intermediate multiplication. */
        unsigned __int128 temp = (unsigned __int128)term * (unsigned __int128)(n - i);
        temp /= (unsigned __int128)(i + 1);
        temp = temp * (unsigned __int128)(5 * n + i + 1);
        temp /= (unsigned __int128)(i + 1);
        term = (seq_t)temp;
#endif
    }
    return sum;
}
