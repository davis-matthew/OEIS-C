/*
 * A243936  Numbers m such that 7 divides A000041(m).
 * https://oeis.org/A243936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer m such that the partition number p(m) is divisible by 7
 * using the pentagonal number theorem recurrence. */
#include <stdlib.h>

/* Helper function to grow an array of seq_t to at least new_cap elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (*cap >= new_cap)
        return;
    size_t new_size = (*cap > 0) ? (*cap * 2) : 4;
    if (new_size < new_cap)
        new_size = new_cap;
    seq_t *tmp = (seq_t *)realloc(*arr, new_size * sizeof(seq_t));
    if (!tmp)
        abort(); /* Allocation failure: abort the program. */
    *arr = tmp;
    *cap = new_size;
}

seq_t a(idx_t n)
{
    /* static state for memoisation across calls */
    static seq_t *pmod      = NULL;      /* partition values modulo 7          */
    static size_t pmod_cap  = 0;         /* current capacity of pmod           */
    static idx_t   pmod_len = 0;         /* number of partition values computed*/

    static seq_t *found     = NULL;      /* list of indices m with p(m) ≡ 0 mod 7 */
    static size_t found_cap = 0;         /* capacity of found array             */
    static size_t found_len = 0;         /* number of found terms               */

    /* Initialise the partition table with p(0) = 1 */
    if (pmod_len == 0) {
        pmod_cap = 1;
        pmod = (seq_t *)malloc(pmod_cap * sizeof(seq_t));
        if (!pmod)
            abort();
        pmod[0] = 1;                /* p(0) = 1 */
        pmod_len = 1;
    }

    /* Initialise found array if not yet allocated */
    if (found_len == 0 && found_cap == 0) {
        found_cap = 4;
        found = (seq_t *)malloc(found_cap * sizeof(seq_t));
        if (!found)
            abort();
    }

    /* Generate terms until we have at least n results */
    while (found_len < n) {
        /* Compute p(pmod_len) modulo 7 */
        idx_t m = pmod_len;            /* new index to compute   */
        seq_t sum = 0;                 /* result modulo 7       */

        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m)
                break;
            idx_t pent2 = k * (3 * k + 1) / 2;
            int sign = (k & 1) ? 1 : -1; /* (-1)^(k-1) */
            seq_t term = pmod[m - pent1];
            if (sign == 1) {
                sum += term;
            } else {
                sum += (seq_t)7 - term;
            }
            sum %= (seq_t)7;

            if (pent2 <= m) {
                term = pmod[m - pent2];
                if (sign == 1) {
                    sum += term;
                } else {
                    sum += (seq_t)7 - term;
                }
                sum %= (seq_t)7;
            }
        }

        /* Store the new value */
        if (pmod_len >= pmod_cap)
            grow_seq(&pmod, &pmod_cap, pmod_len + 1);
        pmod[pmod_len] = sum;

        /* If divisible by 7, record m */
        if (sum == 0) {
            if (found_len >= found_cap)
                grow_seq(&found, &found_cap, found_len + 1);
            found[found_len] = (seq_t)m;
            ++found_len;
        }

        ++pmod_len;
    }

    return found[n - 1];
}
