/*
 * A354540  Number of decorated Dyck paths of length n ending at arbitrary
 *          levels.
 * https://oeis.org/A354540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354540: Number of decorated Dyck paths of length n ending at arbitrary levels.
   Recurrence (valid for n ≥ 7):
     a(n) = [(3n+5)a(n‑1) + 8(2n‑3)a(n‑2) + (23‑17n)a(n‑3) + 2(61‑17n)a(n‑4)
             + (9n‑41)a(n‑5) + 20(n‑6)a(n‑6) + 5(n‑7)a(n‑7)] / (2(n+1))
   Initial values:
     a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=11, a(6)=26.
*/
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    idx_t  new_len   = n + 1;
    seq_t *new_cache = (seq_t *)realloc((void *)cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* out of memory */

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1)
            val = (seq_t)1;
        else if (i == 2)
            val = (seq_t)2;
        else if (i == 3)
            val = (seq_t)3;
        else if (i == 4)
            val = (seq_t)7;
        else if (i == 5)
            val = (seq_t)11;
        else if (i == 6)
            val = (seq_t)26;
        else {
            /* use recurrence for i ≥ 7 */
            int64_t i64   = (int64_t)i;
            seq_t  num    = (seq_t)0;

            /* term1: (3n+5)a(n‑1) */
            int64_t coeff1 = 3 * i64 + 5;
            seq_t  t1 = (seq_t)coeff1 * new_cache[i - 1];
            num += t1;

            /* term2: 8(2n‑3)a(n‑2) */
            int64_t coeff2 = 8 * ((int64_t)2 * i64 - 3);
            seq_t  t2 = (seq_t)coeff2 * new_cache[i - 2];
            num += t2;

            /* term3: (23‑17n)a(n‑3) → subtract */
            int64_t coeff3 = 17 * i64 - 23;
            seq_t  t3 = (seq_t)coeff3 * new_cache[i - 3];
            num -= t3;

            /* term4: 2(61‑17n)a(n‑4) → subtract */
            int64_t coeff4 = 34 * i64 - 122;
            seq_t  t4 = (seq_t)coeff4 * new_cache[i - 4];
            num -= t4;

            /* term5: (9n‑41)a(n‑5) */
            int64_t coeff5 = 9 * i64 - 41;
            seq_t  t5 = (seq_t)coeff5 * new_cache[i - 5];
            num += t5;

            /* term6: 20(n‑6)a(n‑6) */
            int64_t coeff6 = 20 * (i64 - 6);
            seq_t  t6 = (seq_t)coeff6 * new_cache[i - 6];
            num += t6;

            /* term7: 5(n‑7)a(n‑7) */
            int64_t coeff7 = 5 * (i64 - 7);
            seq_t  t7 = (seq_t)coeff7 * new_cache[i - 7];
            num += t7;

            seq_t denom = (seq_t)(2 * (i64 + 1));
            val = num / denom;          /* division is exact */
        }

        new_cache[i] = val;
    }

    cache     = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
