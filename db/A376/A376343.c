/*
 * A376343  Positions of twos in the run-compressed (A037201) first
 *          differences (A001223) of the primes (A000040).
 * https://oeis.org/A376343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n‑th occurrence of 2 in the
 *            run‑compressed first differences of the primes. Computed by
 *            sequentially generating the primes, run‑compressing their first
 *            differences, and recording the positions where the difference
 *            equals 2. (The sequence is the list of these positions.)
 * Range:     u8 n <= 55, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Local helpers ---------------------------------------------------------- */
static void ensure_prime_cap(size_t *cap, uint32_t **list, size_t *len)
{
    if (*len >= *cap) {
        *cap <<= 1;
        uint32_t *tmp = (uint32_t*)realloc(*list, *cap * sizeof(uint32_t));
        if (!tmp) { /* abort on allocation failure */ abort(); }
        *list = tmp;
    }
}

static void ensure_cache_cap(size_t *cap, seq_t **list, size_t *len)
{
    if (*len >= *cap) {
        *cap = (*cap == 0 ? 16 : (*cap << 1));
        seq_t *tmp = (seq_t*)realloc(*list, *cap * sizeof(seq_t));
        if (!tmp) { abort(); }
        *list = tmp;
    }
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* sequence is 1‑based      */

    /* Static state -------------------------------------------------------- */
    static seq_t *two_positions = NULL;        /* cached positions of 2   */
    static size_t pos_cap = 0, pos_len = 0;    /* capacity & length       */

    static uint32_t *prime_list = NULL;        /* generated primes        */
    static size_t prime_cap = 0, prime_len = 0;

    static idx_t next_candidate = 2;           /* next integer to test    */
    static uint32_t prev_prime = 0;            /* previous prime found    */
    static uint32_t last_diff = 0;             /* previous run‑compressed diff */
    static bool last_diff_set = false;         /* flag if last_diff init  */
    static idx_t compressed_pos = 0;           /* current run‑compressed position */
    static idx_t two_seen = 0;                 /* how many 2s found so far */

    /* ------------------------------------------------------------------- */
    /* Ensure the prime list storage is ready.                           */
    if (prime_list == NULL) {
        prime_cap = 256;
        prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_list) abort();
    }

    /* ------------------------------------------------------------------- */
    /* Generate sequence values until we have enough.                      */
    while (pos_len < (size_t)n) {
        /* Find the next prime >= next_candidate                       */
        uint32_t cand = (uint32_t)next_candidate;  /* up to 2^32 enough for n<=10000 */
        bool prime = true;
        uint32_t sq = (uint32_t)(cand * cand);
        for (size_t i = 0; i < prime_len; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }

        if (prime) {
            /* Store prime and resize if needed.                       */
            ensure_prime_cap(&prime_cap, &prime_list, &prime_len);
            prime_list[prime_len++] = cand;

            if (prev_prime == 0) {
                /* First prime (2) – no difference to record.         */
                prev_prime = cand;
            } else {
                uint32_t diff = cand - prev_prime;
                if (!last_diff_set || diff != last_diff) {
                    /* New entry in run‑compressed sequence.          */
                    ++compressed_pos;
                    last_diff = diff;
                    last_diff_set = true;

                    if (diff == 2) {
                        ++two_seen;
                        ensure_cache_cap(&pos_cap, &two_positions, &pos_len);
                        two_positions[pos_len++] = (seq_t)compressed_pos;
                        if (pos_len == (size_t)n) break;
                    }
                }
                prev_prime = cand;
            }
        }

        ++next_candidate;
    }

    return two_positions[n - 1];
}
