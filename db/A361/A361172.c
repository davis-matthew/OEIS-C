/*
 * A361172  a(n) is the smallest positive number not among the terms between
 *          a(n-1) and the previous most recent occurrence of a(n-1)
 *          inclusive; if a(n-1) is a first occurrence, set a(n)=1; a(1)=1.
 * https://oeis.org/A361172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.
   For n > 1, let m = a(n-1) and let p be the previous index
   of m (0 if m is appearing for the first time).
   If p == 0 then a(n) = 1.
   Otherwise a(n) is the smallest positive integer not occurring
   in the sub‑array a(p) … a(n-1) (inclusive). */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc, free, memset */
#include <stdint.h>   /* uint64_t */

/* External integer types defined by the build system. */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* ------------------------------------------------------------------ */
/* Static state for memoisation                                         */
/* ------------------------------------------------------------------ */
static seq_t *seq = NULL;            /* sequence values (1‑based, stored 0‑based) */
static size_t seq_cap = 0;           /* current capacity of seq array       */
static size_t seq_len = 0;           /* number of terms currently stored   */

static idx_t *last_idx  = NULL;       /* most recent index of each value    */
static idx_t *prev_idx  = NULL;       /* previous index of each value       */
static size_t val_cap = 0;            /* capacity for last_idx/prev_idx      */

static unsigned char *seen = NULL;    /* temporary bitmap for mex          */
static size_t seen_cap = 0;           /* capacity of seen array             */

/* ------------------------------------------------------------------ */
/* Ensure we have enough capacity for the sequence array.             */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure we have enough space to index a value 'v'.                  */
static void ensure_val_cap(idx_t v)
{
    size_t tv = (size_t)v;
    if (val_cap <= tv) {
        size_t new_cap = val_cap ? val_cap << 1 : 16;
        while (new_cap <= tv) new_cap <<= 1;
        last_idx = (idx_t *)realloc(last_idx, new_cap * sizeof(idx_t));
        prev_idx = (idx_t *)realloc(prev_idx, new_cap * sizeof(idx_t));
        /* Initialise the new part to zero */
        for (size_t i = val_cap; i < new_cap; ++i) {
            last_idx[i] = 0;
            prev_idx[i] = 0;
        }
        val_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure enough temporary space to mark 'len+1' integers.            */
static void ensure_seen_cap(size_t needed)
{
    if (seen_cap < needed) {
        size_t new_cap = seen_cap ? seen_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seen = (unsigned char *)realloc(seen, new_cap * sizeof(unsigned char));
        seen_cap = new_cap;
    }
    /* Clear the part we will use */
    memset(seen, 0, needed * sizeof(unsigned char));
}

/* ------------------------------------------------------------------ */
/* Main function implementing the sequence.                           */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out‑of‑range request, not used by tests */

    /* Initialise the first term if we have not yet. */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        ensure_val_cap((idx_t)1);
        last_idx[1] = 1;
        prev_idx[1] = 0;
    }

    /* Extend the precomputation until we have n terms. */
    while (seq_len < (size_t)n) {
        /* Current index in 1‑based arithmetic */
        idx_t k = (idx_t)seq_len + 1;

        /* Previous term */
        seq_t m = seq[seq_len - 1];

        /* Find the previous occurrence of m (ignored if none) */
        idx_t prev_occ = prev_idx[(size_t)m];

        seq_t val;
        if (prev_occ == 0) {               /* first occurrence → next term is 1 */
            val = (seq_t)1;
        } else {
            /* Need to look at the range a(prev_occ) … a(k-1) inclusive.   */
            size_t range_len = k - prev_occ;              /* length of the range   */
            size_t seen_needed = range_len + 2;           /* +1 for 1‑based, +1 for mex search */
            ensure_seen_cap(seen_needed);

            /* Mark existing values in the range */
            idx_t start = prev_occ - 1;          /* 0‑based index of first element */
            idx_t end   = (idx_t)seq_len - 1;    /* 0‑based index of last element */
            for (idx_t i = start; i <= end; ++i) {
                seq_t cur = seq[i];
                if (cur <= (seq_t)(range_len + 1)) {
                    seen[(size_t)cur] = 1;
                }
            }

            /* Find the smallest missing positive integer */
            size_t mex = 1;
            while (mex <= range_len + 1 && seen[mex]) ++mex;
            val = (seq_t)mex;
        }

        /* Record the new term and update occurrence indices. */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = val;
        ++seq_len;

        /* Update occurrence info for the new value */
        ensure_val_cap((idx_t)val);
        prev_idx[(size_t)val] = last_idx[(size_t)val];
        last_idx[(size_t)val] = k;
    }

    /* Return the requested term */
    return seq[n - 1];
}
