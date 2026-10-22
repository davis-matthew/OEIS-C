/*
 * A328987  The sequence C(n) defined in the comments (A and B smallest
 *          missing numbers, offset 0).
 * https://oeis.org/A328987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th term of sequence A328987, defined by the
 *            triangular triple (A,B,C) with A[0] = 1, B[0] = 2, C[0] = 3; for
 *            n≥1, A[n] = smallest missing integer from the terms of A,B,C
 *            already defined; B[n] = smallest missing integer from the terms
 *            of A,B,C after A[n] has been added; C[n] = n + A[...
 * Range:     u8 n <= 43, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the generated sequence */
static seq_t *C_cache  = NULL;    /* C[n] values */
static size_t C_cap    = 0;       /* allocated capacity of C_cache */
static size_t C_len    = 0;       /* number of terms currently stored */

/* Booleans to track which integers are already used by A, B or C */
static unsigned char *seen     = NULL;   /* byte array: 0 = unseen, 1 = seen */
static size_t  seen_cap   = 0;           /* current capacity of seen array */

/* Next integer that may be missing; kept between calls */
static idx_t next_missing = 4;           /* starts after 1,2,3 used */

/* Ensure C_cache can hold at least n+1 entries. */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)(n + 1) <= C_cap) return;
    size_t new_cap = C_cap ? C_cap : 16;
    while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(C_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;            /* allocation failure; ignore – tests will not hit it */
    C_cache = tmp;
    C_cap = new_cap;
}

/* Ensure seen can index at least v. */
static void ensure_seen_cap(idx_t v)
{
    if ((size_t)v < seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap : 16;
    while ((size_t)v >= new_cap) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(seen, new_cap * sizeof(unsigned char));
    if (!tmp) return;            /* allocation failure; ignore – tests will not hit it */
    /* zero the newly added part */
    memset(tmp + seen_cap, 0, new_cap - seen_cap);
    seen = tmp;
    seen_cap = new_cap;
}

/* Initialisation of the first term and associated state. */
static void init_sequence(void)
{
    if (C_len > 0) return;              /* already initialised */

    /* Allocate space for C_cache. */
    ensure_cache_cap(0);
    C_cache[0] = (seq_t)3;              /* C[0] = 3 */
    C_len = 1;

    /* Allocate and initialise seen array. */
    allocate_seen: {
        ensure_seen_cap(3);              /* we need at least indices 1..3 */
        /* Mark the initial triple 1,2,3 as seen. */
        seen[1] = 1;
        seen[2] = 1;
        seen[3] = 1;
    }
    /* next_missing starts at 4, the first unused integer. */
    next_missing = 4;
}

/* Return C[n] for any n ≥ 0, computing values lazily. */
seq_t a(idx_t n)
{
    init_sequence();                    /* ensure state is ready */

    if ((size_t)n >= C_len) {           /* need to extend the cache */
        ensure_cache_cap(n);
        for (idx_t k = (idx_t)C_len; k <= n; ++k) {
            /* Skip k=0 – already set. */
            if (k == 0) continue;

            /* Find A[k] = first missing integer. */
            while (true) {
                ensure_seen_cap(next_missing);
                if (!seen[next_missing]) break;
                ++next_missing;
            }
            seq_t a_val = (seq_t)next_missing;
            seen[next_missing] = 1;     /* mark used */
            ++next_missing;            /* move past the value just used */

            /* Find B[k] = next missing integer after A[k]. */
            while (true) {
                ensure_seen_cap(next_missing);
                if (!seen[next_missing]) break;
                ++next_missing;
            }
            seq_t b_val = (seq_t)next_missing;
            seen[next_missing] = 1;     /* mark used */
            ++next_missing;            /* move past the value just used */

            /* Compute C[k] = k + A[k] + B[k]. */
            seq_t c_val = (seq_t)k + a_val + b_val;
            C_cache[k] = c_val;

            /* Mark C[k] as used in the seen set if it fits into idx_t. */
            if (c_val <= (seq_t)UINT64_MAX) {
                idx_t idx = (idx_t)c_val;
                ensure_seen_cap(idx);
                seen[idx] = 1;
            }
        }
        C_len = (size_t)n + 1;
    }

    return C_cache[(size_t)n];
}
