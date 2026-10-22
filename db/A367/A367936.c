/*
 * A367936  Lexicographically earliest sequence of distinct positive integers
 *          such that the sum of the distinct prime factors of a(n) + a(n + 1)
 *          is a composite number.
 * https://oeis.org/A367936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367936: Lexicographically earliest sequence of distinct
   positive integers such that for each n ≥ 1 the sum of the distinct
   prime factors of a(n) + a(n+1) is composite.  The first term is
   a(1) = 1, and for n > 1 the next term is the smallest unused
   positive integer k for which
         sopf(a(n-1) + k)  is a composite number   (sopf = sum of
   distinct prime factors).  The sequence is constructed incrementally,
   memoising values so that any order of calls to a(n) works. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n) */

static seq_t sopf_uint64(uint64_t x)
{
    if (x < 2)
        return 0;
    uint64_t primes[20], exps[20];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i];
    return sum;
}

static bool is_composite(seq_t v)
{
    return (v > 1) && !is_prime_big(v);
}

seq_t a(idx_t n)
{
    /* Invalid index */
    if (n == 0)
        return 0;

    /* State for memoisation */
    static seq_t *seq = NULL;           /* computed terms (0‑based)     */
    static size_t seq_len = 0;          /* number of terms computed    */
    static size_t seq_cap = 0;          /* capacity of seq array       */

    static unsigned char *used = NULL;  /* bitmap: used[i] = 1 if i used */
    static size_t used_cap = 0;         /* maximum index supported      */

    /* Initialise on first call */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;              /* a(1) = 1 */
        seq_len = 1;

        used_cap = 64;
        used = (unsigned char *)calloc(used_cap + 1, 1); /* indices 0..64 */
        used[1] = 1;                                     /* 1 is used */
    }

    /* Grow cache until we have at least n terms */
    while (seq_len < n) {
        /* Increase capacity of seq if necessary */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq = tmp;
            seq_cap = new_cap;
        }

        seq_t prev = seq[seq_len - 1];
        uint64_t candidate = 1;
        uint64_t chosen = 0;

        /* Search for the smallest unused k */
        for (;;) {
            /* Grow used bitmap if needed */
            if (candidate > (uint64_t)used_cap) {
                size_t old_cap = used_cap;
                size_t new_cap = used_cap * 2;
                unsigned char *tmp = (unsigned char *)realloc(used, new_cap + 1);
                memset(tmp + old_cap + 1, 0, new_cap - old_cap);
                used = tmp;
                used_cap = new_cap;
            }

            if (used[(size_t)candidate])
                ++candidate;            /* already used */
            else {
                /* Compute sum of distinct prime factors of prev + candidate */
                uint64_t sum64 = (uint64_t)prev + candidate;
                seq_t sopf_val = sopf_uint64(sum64);
                if (is_composite(sopf_val)) {
                    chosen = candidate;
                    break;
                }
                ++candidate;
            }
        }

        /* Record the chosen term */
        seq[seq_len] = (seq_t)chosen;
        used[(size_t)chosen] = 1;
        ++seq_len;
    }

    return seq[n - 1];
}
