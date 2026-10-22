/*
 * A387287  Primes in the order of their first appearance among the factors of
 *          the averages of twin prime pairs.
 * https://oeis.org/A387287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) computed by a search for twin‑prime averages and collecting
 *            the first new prime factor in each order
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x);          */
#include "../utils/factor_u64.h" /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

static seq_t   *seq   = NULL;   /* cache of the sequence values      */
static size_t   seq_cap = 0;    /* allocated capacity of seq        */
static size_t   seq_len = 0;    /* number of values stored          */

static bool   *seen_flag = NULL;  /* seen_flag[p] == true if prime p
                                   already appeared as a factor      */
static size_t   seen_cap = 0;     /* allocated capacity of seen_flag */

static uint64_t next_p = 3;       /* next odd candidate for twin prime */

/* Ensure seen_flag can be indexed at least up to min_cap-1.  */
static void grow_seen_capacity(uint64_t min_cap)
{
    if (min_cap <= seen_cap)
        return;
    size_t new_cap = seen_cap ? seen_cap : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(seen_flag, new_cap * sizeof(bool));
    if (!tmp) exit(1);
    memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
    seen_flag = tmp;
    seen_cap  = new_cap;
}

/* Append a value to seq, growing the array if needed.  */
static void append_seq(seq_t val)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 4;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* outside OEIS offset – return 0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        uint64_t p = next_p;

        /* Check whether (p, p+2) is a twin prime pair.  */
        if (is_prime(p) && is_prime(p + 2)) {
            uint64_t avg = p + 1;            /* average of the twin pair */

            /* Factor the average; we need only its distinct prime factors. */
            uint64_t fac_pr[64];
            uint64_t fac_ex[64];
            size_t fac_cnt;
            factor_u64(avg, fac_pr, fac_ex, &fac_cnt);

            for (size_t i = 0; i < fac_cnt && seq_len < (size_t)n; ++i) {
                uint64_t q = fac_pr[i];
                grow_seen_capacity(q + 1);    /* ensure seen_flag can index q */

                if (!seen_flag[q]) {
                    seen_flag[q] = true;
                    append_seq((seq_t)q);
                }
            }
        }
        next_p += 2;   /* advance to the next odd number */
    }
    return seq[n - 1];
}
