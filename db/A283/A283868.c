/*
 * A283868  For all n, the set including the terms {a(1), a(2), a(3), ...,
 *          a(n)} has a nonprime number of digits 0, 1, 2, 3, 4, 5, 6, 7, 8
 *          and 9.
 * https://oeis.org/A283868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A283868
   Construct the sequence by starting with 1 and repeatedly appending
   the smallest integer not yet present that keeps every digit count
   (0–9) a non‑prime number.  The function a(idx_t n) returns the nth
   term (n ≥ 1).  Sequence values are stored in static arrays so that
   calls in any order are O(1) after the needed terms have been
   generated. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/get_digits.h"

/* --------------------------------------------------------------------- */
/* Small primality test for 32‑bit values (used to test digit‑count
   primality). */
static bool is_prime_uint32(uint32_t n)
{
    if (n < 2u) return false;
    if (n % 2u == 0) return n == 2u;
    for (uint32_t d = 3u; d * d <= n; d += 2u)
        if (n % d == 0) return false;
    return true;
}

/* --------------------------------------------------------------------- */
/* Dynamic storage for the sequence values. */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static void ensure_seq_capacity(size_t needed)
{
    if (!seq) {
        seq_cap = 256;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    if (needed > seq_cap) {
        while (seq_cap < needed) seq_cap <<= 1;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
}

/* --------------------------------------------------------------------- */
/* Bitset recording which integers have already appeared. */
static uint8_t *used     = NULL;
static size_t   used_cap = 0;

static void ensure_used_capacity(size_t needed)
{
    if (!used) {
        used_cap = 1024;
        used = (uint8_t *)calloc(used_cap, 1);
    }
    if (needed >= used_cap) {
        size_t new_cap = used_cap;
        while (new_cap <= needed) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)calloc(new_cap, 1);
        memcpy(tmp, used, used_cap);
        free(used);
        used = tmp;
        used_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Global digit counts for the current set of terms. */
static uint64_t digitCnt[10] = {0};

/* --------------------------------------------------------------------- */
/* The smallest integer that has not yet appeared (increasing over time). */
static size_t min_unused = 1;

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms.  The conversion from
       idx_t to size_t is safe for the intended n (≤ ~1e5). */
    while (seq_len < (size_t)n)
    {
        /* Find the smallest admissible candidate. */
        size_t cand = min_unused;
        for (;; ++cand)
        {
            /* Make sure the used array can address cand. */
            ensure_used_capacity(cand);

            if (used[cand]) continue;  /* already in the sequence */

            /* Count the decimal digits of the candidate. */
            unsigned char cnt[10] = {0};
            uint8_t buf[64];
            size_t dlen = 0;
            get_digits((seq_t)cand, buf, &dlen);
            for (size_t i = 0; i < dlen; ++i)
                cnt[buf[i]]++;

            /* Verify that adding this candidate keeps all digit counts
               non‑prime. */
            bool ok = true;
            for (int d = 0; d < 10 && ok; ++d)
            {
                uint64_t newcnt = digitCnt[d] + cnt[d];
                if (is_prime_uint32((uint32_t)newcnt))
                    ok = false;
            }

            if (!ok) continue;   /* try the next integer */

            /* Accept the candidate. */
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len] = (seq_t)cand;
            seq_len++;

            /* Mark it as used. */
            used[cand] = 1;

            /* Advance min_unused if we just used it. */
            if (cand == min_unused)
            {
                do {
                    ++min_unused;
                    ensure_used_capacity(min_unused);
                } while (used[min_unused]);
            }

            /* Update digit counts. */
            for (int d = 0; d < 10; ++d)
                digitCnt[d] += cnt[d];

            break;  /* go on to the next term */
        }
    }
    return seq[n - 1];
}
