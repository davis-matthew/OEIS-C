/*
 * A381290  Number of subsets of 6 integers between 1 and n such that their
 *          sum is 1 modulo n.
 * https://oeis.org/A381290
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n choose 6) + (n/2 choose 3) - (n/3 choose 2) - n/6) /
 *            n (terms appear only when the divisibility conditions hold)
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* size_t, NULL */

/* compute binomial coefficient (n choose k) for k <= 6 using 128‑bit integers
   to avoid overflow for n up to 10^4 (and still works for larger n). */
static __int128 binom128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (__int128)(n - k + i);
        res /= (__int128)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 7) return (seq_t)0;

    __int128 sum = 0;

    /* term for m = 1  (g = n) */
    sum += binom128(n, 6);

    /* term for m = 2  (g = n/2) – appears only if n is even */
    if ((n & 1) == 0) {
        sum += binom128(n / 2, 3);
    }

    /* term for m = 3  (g = n/3) – appears only if n divisible by 3 */
    if (n % 3 == 0) {
        sum -= binom128(n / 3, 2);
    }

    /* term for m = 6  (g = n/6) – appears only if n divisible by 6 */
    if (n % 6 == 0) {
        sum -= (__int128)(n / 6);
    }

    /* a(n) is guaranteed to be an integer; perform exact division */
    __int128 result = sum / (__int128)n;
    return (seq_t)result;
}
