/*
 * A270269  Prime numbers with locations of right angle turns in the Ulam
 *          square spiral that are vertices of isosceles right triangles.
 * https://oeis.org/A270269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For k ≥ 1 let L = 2k+1 and c0 = L², c1 = L² − 2k, c2 = L² − 4k,
 *            c3 = L² − 6k. These are the four corner numbers of the Ulam
 *            square spiral. If at least three of the corners are prime, then
 *            all such primes are vertices of an isosceles right triangle and
 *            belong to the sequence A270269. The primes fo...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   grow_seq, is_prime_big, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/grow_seq.h"

static seq_t *seq   = NULL;   /* cached sequence values          */
static size_t seq_len = 0;    /* number of values in seq         */
static size_t seq_cap = 0;    /* allocated capacity of seq       */
static idx_t   next_k  = 1;   /* next layer to examine           */

static void ensure_cache(idx_t n)
{
    /* Grow the cached sequence until it contains at least n terms. */
    while (seq_len < (size_t)n)
    {
        /* Compute the four corner values for layer k = next_k. */
        seq_t L  = (seq_t)(2 * next_k + 1);
        seq_t L2 = L * L;

        seq_t c[4];
        c[0] = L2;
        c[1] = L2 - (seq_t)(2 * next_k);
        c[2] = L2 - (seq_t)(4 * next_k);
        c[3] = L2 - (seq_t)(6 * next_k);

        int prime_flag[4] = {0};
        int prime_cnt     = 0;

        for (int i = 0; i < 4; ++i)
        {
            if (is_prime_big(c[i]))
            {
                prime_flag[i] = 1;
                ++prime_cnt;
            }
        }

        /* Only add primes when at least three corners are prime. */
        if (prime_cnt >= 3)
        {
            /* Append primes in ascending order: c3, c2, c1, c0. */
            for (int i = 3; i >= 0; --i)
            {
                if (prime_flag[i])
                {
                    if (seq_len == seq_cap)
                        grow_seq(&seq, &seq_cap, seq_len + 3);
                    seq[seq_len++] = c[i];
                }
            }
        }

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is 1‑based; undefined for n=0 */

    if (seq == NULL)
    {
        seq_cap = 1024;
        seq     = seq_alloc(seq_cap);
        seq_len = 0;
    }

    ensure_cache(n);
    return seq[n - 1];
}
