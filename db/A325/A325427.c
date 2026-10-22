/*
 * A325427  a(n) is the least number not 2*a(m)+1 or floor(3*a(m)/2) for any m
 *          < n.
 * https://oeis.org/A325427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least number not equal to 2*a(m)+1 * or floor(3*a(m)/2)
 *            for any m < n (offset 1)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static memoized storage for the sequence and forbidden values */
static seq_t *seq = NULL;                   /* sequence values */
static size_t seq_len = 0;                  /* number of terms stored */
static size_t seq_cap = 0;                  /* allocated capacity of seq */

static unsigned char *forbidden = NULL;     /* forbidden numbers bitmap (0=free,1=forbidden) */
static size_t forbid_cap = 0;               /* allocated size of forbidden array */
static idx_t next_candidate = 1;            /* next candidate integer to try */

/* Ensure that seq has at least "needed" capacity */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure that forbidden has at least "needed" capacity and zero‑initialize new slots */
static void ensure_forbid_cap(size_t needed)
{
    if (needed <= forbid_cap) return;
    size_t new_cap = forbid_cap ? forbid_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(forbidden, new_cap);
    if (!tmp) abort();
    /* zero new portion */
    memset(tmp + forbid_cap, 0, new_cap - forbid_cap);
    forbidden = tmp;
    forbid_cap = new_cap;
}

/* Find the smallest positive integer not marked as forbidden */
static seq_t next_free_value(void)
{
    /* Advance candidate until it is not forbidden */
    while ((size_t)next_candidate < forbid_cap && forbidden[(size_t)next_candidate]) {
        ++next_candidate;
    }
    return next_candidate;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* outside defined offset */

    /* If we already have the requested term, return it */
    if ((size_t)n <= seq_len) {
        return seq[n - 1];
    }

    /* Ensure storage for n terms */
    ensure_seq_cap((size_t)n);

    /* Generate terms up to n */
    for (size_t idx = seq_len; idx < (size_t)n; ++idx) {
        /* Find next available integer */
        seq_t val = next_free_value();

        /* Store it */
        seq[idx] = val;
        seq_len = idx + 1;

        /* Mark forbidden values derived from this term */
        seq_t x1 = (val << 1) + 1;         /* 2*val + 1 */
        seq_t x2 = (val * 3) / 2;          /* floor(3*val/2) */

        /* Mark x1 */
        if ((size_t)x1 > forbid_cap) {
            ensure_forbid_cap((size_t)x1 + 1);
        }
        forbidden[(size_t)x1] = 1;

        /* Mark x2 */
        if ((size_t)x2 > forbid_cap) {
            ensure_forbid_cap((size_t)x2 + 1);
        }
        forbidden[(size_t)x2] = 1;

        /* Move candidate past the value just used */
        ++next_candidate;
        while ((size_t)next_candidate < forbid_cap && forbidden[(size_t)next_candidate]) {
            ++next_candidate;
        }
    }

    return seq[n - 1];
}
