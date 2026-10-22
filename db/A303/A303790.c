/*
 * A303790  G.f. satisfies: 120*(1-216*x)*A(x) + (1-3*(1-216*x)^2)*A'(x) -
 *          (1-216*x)*(2-216*x)*x*A''(x) = 0, a(0)=1.
 * https://oeis.org/A303790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*(27*n^2-27*n+5)*a(n-1) - 2592*(3*n-5)*(3*n-1)*a(n-2))
 *            / n^2 with initial values a(0)=1, a(1)=60 and a(n)=a(n) for n≥2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper multiplication routine.
   In C++ builds we use uint128_t for 64‑bit values to avoid overflow,
   otherwise (arbitrary‑precision) the built‑in multiplication of seq_t is used. */
#ifdef __cplusplus
#include <type_traits>
static inline seq_t mul_uint128(seq_t a, seq_t b) {
    if constexpr (std::is_same<seq_t, unsigned long long>::value) {
        unsigned __int128 r = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)r;
    } else {
        return a * b;
    }
}
#else
static inline seq_t mul_uint128(seq_t a, seq_t b) {
    unsigned __int128 r = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)r;
}
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)60;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)60;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;

        /* c1 = 27*i^2 - 27*i + 5 */
        seq_t c1 = mul_uint128((seq_t)27, mul_uint128(i_seq, i_seq));
        c1 = c1 - mul_uint128((seq_t)27, i_seq);
        c1 = c1 + (seq_t)5;

        /* term1 = 12 * c1 * prev1 */
        seq_t term1 = mul_uint128((seq_t)12, mul_uint128(c1, prev1));

        /* term2 = 2592 * (3*i-5) * (3*i-1) * prev2 */
        seq_t tA = (seq_t)(3 * i) - (seq_t)5;
        seq_t tB = (seq_t)(3 * i) - (seq_t)1;
        seq_t term2 = mul_uint128((seq_t)2592,
                                 mul_uint128(tA,
                                             mul_uint128(tB, prev2)));

        /* numerator and denominator */
        seq_t num = term1 - term2;
        seq_t den = mul_uint128(i_seq, i_seq);

        /* exact division */
        seq_t cur = num / den;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
