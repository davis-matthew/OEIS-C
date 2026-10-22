/*
 * A350142  Numbers m of the form 2^k + 1 such that tau(m-2) = tau(m-1) - 1.
 * https://oeis.org/A350142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A350142
   a(n) = 2^k + 1 where k = 1, 2, 4, 6, 8, 16, 32, … .
   The only non‑power‑of‑two solution is k = 6 which occurs at n = 4.
   For n ≥ 5 we have k = 2^(n‑2).  All arithmetic is done on seq_t. */

#include "../utils/pow2.h"  /* pow2(idx_t exp) returns 2^exp as seq_t */
#include "../utils/big_pow.h" /* big_pow(seq_t base, seq_t exp) */

/* external linkage as requested */
seq_t a(idx_t n)
{
    /* handle the first four special cases */
    if (n == 1) return (seq_t)3;       /* k = 1  → 2^1 + 1 = 3   */
    if (n == 2) return (seq_t)5;       /* k = 2  → 2^2 + 1 = 5   */
    if (n == 3) return (seq_t)17;      /* k = 4  → 2^4 + 1 = 17  */
    if (n == 4) return (seq_t)65;      /* k = 6  → 2^6 + 1 = 65  */

    /* for n ≥ 5 we use k = 2^(n‑2) */
    idx_t exp = n - 2;                 /* exponent for the inner power of 2 */

    /* protect against ridiculous shift values */
    if (exp >= sizeof(idx_t) * 8) {
        /* value too large to represent as an exponent; not expected in tests */
        return (seq_t)0;
    }

    /* compute k = 2^(n‑2) as a 64‑bit unsigned value */
    idx_t k = (idx_t)1 << exp;          /* works because exp < 64 */

    /* compute m = 2^k + 1 */
    seq_t m = big_pow((seq_t)2, (seq_t)k); /* 2^k */
    m += (seq_t)1;                       /* +1 */

    return m;
}
