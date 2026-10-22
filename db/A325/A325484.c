/*
 * A325484  One of the four successive approximations up to 5^n for the 5-adic
 *          integer 6^(1/4). This is the 1 (mod 5) case (except for n = 0).
 * https://oeis.org/A325484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; for n>1, a(n) is the unique integer k in [1,5^n]
 *            with k≡1 (mod 5) and k^4≡6 (mod 5^n). The values are obtained by
 *            successive Hensel lifting: kₙ₊₁ = kₙ + t·5ⁿ, where t ≡
 *            –((kₙ^4−6)/5ⁿ)·4⁻¹ (mod 5). The implementation memoises
 *            already‑computed terms and is O(n) per request, using on...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Memoisation table */
    static seq_t *memo      = NULL;   /* array of seq_t */
    static size_t memo_cap  = 0;      /* allocated capacity (elements) */
    static size_t memo_len  = 0;      /* number of values stored */

    /* Current lift state */
    static seq_t cur_k   = 0;        /* k_n for current n */
    static seq_t cur_pow5 = 1;       /* 5^current_n */

    /* Initialise on first call */
    if (memo_len == 0) {
        memo_cap = 8;                     /* initial capacity */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return 0;              /* allocation failed – abort */
        memo[0] = (seq_t)0;               /* a(0) = 0   */
        memo[1] = (seq_t)1;               /* a(1) = 1   */
        memo_len = 2;
        cur_k    = (seq_t)1;              /* a(1)          */
        cur_pow5 = (seq_t)5;              /* 5^1           */
    }

    /* Return already computed value if possible */
    if (n < memo_len)
        return memo[n];

    /* Extend memoisation up to requested n */
    for (size_t idx = memo_len; idx <= (size_t)n; ++idx) {
        /* next_pow5 = 5^(idx)  (i.e. 5^(current_n+1)) */
        seq_t next_pow5 = cur_pow5 * (seq_t)5;

        /* (cur_k)^4 mod next_pow5 */
        seq_t k4_mod = mod_pow(cur_k, (uint64_t)4, next_pow5);

        /* Compute m = ((k^4 - 6) / 5^current_n) mod 5
           Since k4_mod < next_pow5, we can extract the quotient
           directly after correcting for possible underflow. */
        seq_t tmp;
        if (k4_mod >= (seq_t)6)
            tmp = k4_mod - (seq_t)6;
        else
            tmp = k4_mod + next_pow5 - (seq_t)6;   /* ensures 0 ≤ tmp < next_pow5 */

        /* tmp / cur_pow5 gives m (value 0..4) */
        seq_t m = tmp / cur_pow5;

        /* t = -(m * 4) mod 5  (i.e. t ≡ -(m*4) (mod 5)) */
        seq_t t = (seq_t)((5 - ((m * (seq_t)4) % (seq_t)5)) % (seq_t)5);

        /* Lift to next level: k_{idx} = k_{idx-1} + t * 5^(idx-1) */
        seq_t new_k = cur_k + t * cur_pow5;

        /* Grow memoisation array if needed */
        if (memo_len == memo_cap) {
            memo_cap *= 2;
            seq_t *tmp_ptr = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
            if (!tmp_ptr) { free(memo); return 0; } /* allocation failed */
            memo = tmp_ptr;
        }

        memo[memo_len++] = new_k;
        cur_k   = new_k;
        cur_pow5 = next_pow5;
    }

    return memo[n];
}
