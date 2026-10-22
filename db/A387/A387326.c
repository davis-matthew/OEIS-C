/*
 * A387326  Numbers whose prime factors do not have choosable sets of integer
 *          partitions.
 * https://oeis.org/A387326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387326:
   a(n) is the n-th integer that has a prime factor p with
   exponent e > number of integer partitions of p.
   (For primes 2,3,5,7 the relevant thresholds are 3,4,8,16 respectively.)
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *seq   = NULL;   /* cache of sequence values      */
static size_t  seq_len = 0;   /* number of values cached       */
static size_t  seq_cap = 0;   /* capacity of seq array         */

static uint64_t *spf   = NULL;   /* smallest prime factor array   */
static size_t  spf_cap = 0;      /* capacity of spf array         */

static uint64_t last_checked = 0; /* highest integer examined so far */

/* Expand the SPF array to include at least 'need' */
static void extend_spf_if_needed(uint64_t need)
{
    if (need <= spf_cap) return;

    size_t new_limit = spf_cap ? spf_cap * 2 : 1024;
    while (new_limit <= need) new_limit *= 2;

    /* free old table if any */
    free(spf);
    spf = (uint64_t *)calloc(new_limit + 1, sizeof(uint64_t));
    if (!spf) return;           /* OOM, ignore for brevity */

    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = i;
            for (uint64_t j = i * i; j <= new_limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (size_t i = 2; i <= new_limit; ++i)
        if (spf[i] == 0) spf[i] = i;

    spf_cap = new_limit;
}

/* Append a value into the cached sequence, growing if needed. */
static void append_seq(uint64_t val)
{
    if (seq_len + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_arr) return;            /* OOM, ignore for brevity */
        seq = new_arr;
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)val;
}

/* Test whether integer x satisfies the A387326 property. */
static bool bad_exponent(uint64_t x)
{
    while (x > 1) {
        uint64_t p = spf[x];
        uint64_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        if (p == 2ULL && cnt > 2ULL) return true;
        if (p == 3ULL && cnt > 3ULL) return true;
        if (p == 5ULL && cnt > 7ULL) return true;
        if (p == 7ULL && cnt > 15ULL) return true;
        /* Other primes cannot reach their thresholds within 64‑bit range. */
    }
    return false;
}

/* Public function required by the testing harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but avoid UB */

    /* generate sequence values until we have at least n of them */
    for (uint64_t candidate = last_checked + 1;
         seq_len < n;
         ++candidate) {

        extend_spf_if_needed(candidate);
        if (bad_exponent(candidate))
            append_seq(candidate);

        last_checked = candidate;
    }

    return seq[n - 1];
}
