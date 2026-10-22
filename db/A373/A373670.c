/*
 * A373670  Numbers k such that the k-th run-length A110969(k) of the sequence
 *          of non-prime-powers (A024619) is different from all prior
 *          run-lengths.
 * https://oeis.org/A373670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   grow_to_cap, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A373670:
   Numbers k such that the k‑th run‑length of the sequence of non‑prime‑powers
   (A024619) is different from all prior run‑lengths.  The function finds
   the lengths of consecutive runs of non‑prime‑powe­r integers and records
   the run index whenever a new length appears. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>                     /* only for size_t, not arithmetic on seq_t */

/* Integer square root helper (provided by the framework). */
#include "../utils/int_sqrt.h"
#include "../utils/grow_to_cap.h"

/* --------------------------------------------------------------------- */
/* Prime generation helpers                                            */
/* --------------------------------------------------------------------- */
static uint32_t *prime_list   = NULL;   /* stored primes (uint32_t suffices) */
static size_t   prime_cnt     = 0;      /* number of stored primes       */
static size_t   prime_cap     = 0;      /* allocated capacity            */
static uint64_t prime_last_checked = 1; /* largest number tested so far   */

/* Ensure that all primes ≤ limit are generated and stored in prime_list. */
static void generate_primes_up_to(uint64_t limit)
{
    if (limit <= prime_last_checked) return;

    for (uint64_t n = prime_last_checked + 1; n <= limit; ++n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)n;
        }
    }
    prime_last_checked = limit;
}

/* --------------------------------------------------------------------- */
/* Test whether n is a prime power (including primes). 1 is not a prime
   power.                                                       */
/* --------------------------------------------------------------------- */
static bool is_prime_power(uint64_t n)
{
    if (n <= 1) return false;

    uint64_t remaining = n;
    uint64_t first_factor = 0;

    /* Ensure we have all primes up to √n. */
    uint64_t lim = int_sqrt(n);
    generate_primes_up_to(lim);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > remaining) break;
        if (remaining % p == 0) {
            if (first_factor == 0) first_factor = p;
            else if (p != first_factor) return false;
            while (remaining % p == 0) remaining /= p;
        }
    }

    /* Remaining factor > 1 is a prime > √n.           */
    if (remaining > 1) {
        if (first_factor == 0) first_factor = remaining;
        else if (remaining != first_factor) return false;
    }

    return true;
}

/* --------------------------------------------------------------------- */
/* Resize helpers                                                       */

/* --------------------------------------------------------------------- */
/* Sequence storage                                                      */
/* --------------------------------------------------------------------- */
static seq_t *seq_out      = NULL;
static size_t seq_out_cap  = 0;
static size_t seq_out_len  = 0;

/* --------------------------------------------------------------------- */
/* At‑least as many runs as needed for current maximum run length.      */
/* --------------------------------------------------------------------- */
static bool *len_seen  = NULL;  /* true if a length has been seen       */
static size_t len_seen_cap = 0; /* allocated capacity (in lengths)     */

static void ensure_len_seen_cap(size_t len)
{
    if (len >= len_seen_cap) {
        size_t new_cap = (len_seen_cap == 0) ? 64 : len_seen_cap;
        while (new_cap <= len) new_cap <<= 1;
        bool *tmp = (bool*)realloc(len_seen, new_cap * sizeof(bool));
        if (!tmp) abort();
        memset(tmp + len_seen_cap, 0, (new_cap - len_seen_cap) * sizeof(bool));
        len_seen = tmp;
        len_seen_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset safety; actual sequence starts at 1 */

    /* Persistent state across successive calls. */
    static uint64_t cur_num      = 1;   /* next integer to examine          */
    static size_t   cur_run_len  = 0;   /* length of the run currently being built  */
    static size_t   run_index    = 0;   /* number of runs processed so far          */

    /* Generate runs until we have at least n terms in seq_out. */
    while (seq_out_len < (size_t)n) {
        if (!is_prime_power(cur_num)) {          /* non‑prime‑power => extend run */
            ++cur_run_len;
        } else {                                 /* prime power => run ends       */
            if (cur_run_len > 0) {
                ++run_index;                     /* the run we just finished          */
                ensure_len_seen_cap(cur_run_len);
                if (!len_seen[cur_run_len]) {    /* new length seen for the first time */
                    len_seen[cur_run_len] = true;
                    seq_out = (seq_t*)grow_to_cap(seq_out, &seq_out_cap,
                                                   seq_out_len + 1, sizeof(seq_t));
                    seq_out[seq_out_len++] = (seq_t)run_index;
                }
                cur_run_len = 0;                   /* start new run */
            }
        }
        ++cur_num;
    }

    return seq_out[n - 1];
}
