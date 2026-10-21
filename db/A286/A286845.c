/*
 * A286845  Every 3-digit number 'a' such that there exist two other 3-digit
 *          numbers, 'b' and 'c', such that a - b = c, and a,b,c collectively
 *          use every digit 1-9 exactly once.
 * https://oeis.org/A286845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = nth smallest 3‑digit number a for which there exist
 *            distinct * 3‑digit numbers b, c using the remaining digits 1‑9
 *            exactly once such that * a - b = c. This function enumerates all
 *            permutations of the digits 1–9, * checks the condition, stores
 *            the corresponding a values, sorts them, * and r...
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* array of unique a values, sorted ascending */
static size_t cache_len = 0;    /* number of terms stored */

/* Recursive permutation generator.
   pos: current position in the permutation (0..8)
   used: bitmask of digits already used (bit d corresponds to digit d)
   perm: current permutation being built
   out: array to store matching a values
   cnt: number of matches found so far */
static void gen_perm(int pos, uint16_t used, uint8_t perm[9],
                     seq_t *out, size_t *cnt)
{
    if (pos == 9) {
        int a = perm[0] * 100 + perm[1] * 10 + perm[2];
        int b = perm[3] * 100 + perm[4] * 10 + perm[5];
        int c = perm[6] * 100 + perm[7] * 10 + perm[8];
        if (a > b && (a - b) == c) {
            out[*cnt] = (seq_t)a;
            ++*cnt;
        }
        return;
    }
    for (int d = 1; d <= 9; ++d) {
        if (!(used & (1U << d))) {
            perm[pos] = (uint8_t)d;
            gen_perm(pos + 1, used | (1U << d), perm, out, cnt);
        }
    }
}

/* Initialise the sequence cache on first call. */
static void init_sequence(void)
{
    if (cache)   /* already initialised */
        return;

    const int MAX_PERM = 362880; /* 9! permutations */
    cache = (seq_t *)malloc(MAX_PERM * sizeof(seq_t));
    if (!cache)   /* allocation failed, return without setting cache_len */
        return;

    size_t temp_cnt = 0;
    uint8_t perm[9];
    gen_perm(0, 0, perm, cache, &temp_cnt);

    /* Simple insertion sort – the array is small (≤362880). */
    for (size_t i = 1; i < temp_cnt; ++i) {
        size_t j = i;
        while (j > 0 && cache[j - 1] > cache[j]) {
            seq_t t = cache[j - 1];
            cache[j - 1] = cache[j];
            cache[j]     = t;
            --j;
        }
    }

    /* Remove duplicates (same a can arise from different b/c orderings). */
    size_t uniq = 0;
    for (size_t i = 0; i < temp_cnt; ++i) {
        if (uniq == 0 || cache[i] != cache[uniq - 1]) {
            cache[uniq++] = cache[i];
        }
    }
    cache_len = uniq;
}

/* Return the nth term of the sequence.  n is 1‑based offset. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    init_sequence();
    if (!cache || (idx_t)(n - 1) >= cache_len)
        return (seq_t)0;   /* undefined term */

    return cache[n - 1];
}
