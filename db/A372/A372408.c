/*
 * A372408  a(n) = smallest composite not occurring earlier having in decimal
 *          representation to its predecessor Levenshtein distance = 1;
 *          a(1)=1.
 * https://oeis.org/A372408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest unused composite whose decimal
 *            representation differs from that of a(n-1) by Levenshtein
 *            distance 1. a(1)=1. The function builds the sequence lazily and
 *            remembers all previously generated terms
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/is_prime_small.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *seq = NULL;          /* dynamically growing sequence */
static size_t seqSize = 0;
static size_t seqCap = 0;

static uint8_t *used = NULL;       /* bitmap of used numbers up to the maximum candidate */
static size_t usedCap = 0;

/* Ensure the allocated sequence has at least 'minCap' slots. */
static void ensure_seq_cap(size_t minCap)
{
    if (seqCap < minCap)
    {
        size_t newCap = seqCap ? seqCap * 2 : 16;
        if (newCap < minCap) newCap = minCap;
        seq = seq_realloc(seq, seqCap, newCap);
        seqCap = newCap;
    }
}

/* Expand the used bitmap so that indices up to 'x' are valid. */
static void ensure_used_cap(idx_t x)
{
    while (usedCap <= (size_t)x)
    {
        size_t newCap = usedCap ? usedCap * 2 : 1024;
        uint8_t *tmp = (uint8_t*)realloc(used, newCap);
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        memset(tmp + usedCap, 0, newCap - usedCap);
        used = tmp;
        usedCap = newCap;
    }
}

/* Convert an idx_t number into its decimal string representation. */
static int idx_to_str(idx_t x, char *out)
{
    if (x == 0)
    {
        out[0] = '0';
        out[1] = '\0';
        return 1;
    }
    char tmp[32];
    int pos = 0;
    while (x > 0)
    {
        tmp[pos++] = '0' + (x % 10);
        x /= 10;
    }
    for (int i = 0; i < pos; ++i)
        out[i] = tmp[pos - 1 - i];
    out[pos] = '\0';
    return pos;
}

/* Check if an idx_t value is a composite number (i.e. not <4 and not prime). */
static bool is_composite(idx_t x)
{
    if (x < 4) return false;          /* 1,2,3 are not composite */
    return !is_prime_small((seq_t)x);
}

/* Find the smallest unused composite at edit‑distance 1 from 'prev'. */
static idx_t find_next(idx_t prev)
{
    char prev_str[32];
    int len = idx_to_str(prev, prev_str);
    char cand[32];
    idx_t best = (idx_t)-1;

    /* Substitution */
    for (int i = 0; i < len; ++i)
    {
        char orig = prev_str[i];
        for (char d = '0'; d <= '9'; ++d)
        {
            if (d == orig) continue;
            memcpy(cand, prev_str, len);
            cand[i] = d;
            cand[len] = '\0';
            idx_t val = (idx_t)strtoull(cand, NULL, 10);
            if (!is_composite(val)) continue;
            ensure_used_cap(val);
            if (used[(size_t)val]) continue;
            if (best == (idx_t)-1 || val < best) best = val;
        }
    }

    /* Insertion */
    for (int pos = 0; pos <= len; ++pos)
    {
        for (char d = '0'; d <= '9'; ++d)
        {
            int j = 0;
            for (int i = 0; i < pos; ++i) cand[j++] = prev_str[i];
            cand[j++] = d;
            for (int i = pos; i < len; ++i) cand[j++] = prev_str[i];
            cand[j] = '\0';
            idx_t val = (idx_t)strtoull(cand, NULL, 10);
            if (!is_composite(val)) continue;
            ensure_used_cap(val);
            if (used[(size_t)val]) continue;
            if (best == (idx_t)-1 || val < best) best = val;
        }
    }

    /* Deletion (only if length > 1) */
    if (len > 1)
    {
        for (int pos = 0; pos < len; ++pos)
        {
            int j = 0;
            for (int i = 0; i < len; ++i)
            {
                if (i == pos) continue;
                cand[j++] = prev_str[i];
            }
            cand[j] = '\0';
            idx_t val = (idx_t)strtoull(cand, NULL, 10);
            if (!is_composite(val)) continue;
            ensure_used_cap(val);
            if (used[(size_t)val]) continue;
            if (best == (idx_t)-1 || val < best) best = val;
        }
    }

    return best;
}

/* Public function that returns a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    if (seqSize == 0)
    {
        seqCap = 16;
        seq = seq_alloc(seqCap);
        seq[0] = (seq_t)1;
        seqSize = 1;

        usedCap = 1024;
        used = (uint8_t*)malloc(usedCap);
        if (!used) exit(EXIT_FAILURE);
        memset(used, 0, usedCap);
        used[1] = 1;   /* mark the first term as used */
    }

    while (seqSize < n)
    {
        idx_t prev = (idx_t)seq[seqSize - 1];
        idx_t next = find_next(prev);
        if (next == (idx_t)-1) exit(EXIT_FAILURE);  /* should not happen */
        ensure_seq_cap(seqSize + 1);
        seq[seqSize] = (seq_t)next;
        ensure_used_cap(next);
        used[(size_t)next] = 1;
        ++seqSize;
    }
    return seq[n - 1];
}
