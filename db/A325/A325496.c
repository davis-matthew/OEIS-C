/*
 * A325496  Difference sequence of A077477.
 * https://oeis.org/A325496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A077477(n+1) - A077477(n), where A077477 is defined by *
 *            A(1) = 1 and for n>1, A(n) is the least integer not equal to *
 *            2*A(m)+1 or 3*A(m)+1 for any m < n. * This implementation builds
 *            the sequence lazily, keeping the array * of A(·) values and a
 *            boolean table of forbidden numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool          initialized = false;   /* indicates that globals are ready   */
static size_t        a_cap = 0;             /* capacity of A array                */
static size_t        a_len = 0;             /* number of A values computed        */
static seq_t        *Aarr = NULL;           /* array holding A(1), A(2), …        */
static size_t        forb_cap = 0;          /* capacity of forbidden table        */
static bool         *forbidden = NULL;      /* flag array: forbidden[i] == true  */
static idx_t          next_candidate = 1;   /* next number to try for A value     */

/* Ensure forbidden array can store index idx (0‑based). */
static inline void
ensure_forbidden(idx_t idx)
{
    if ((size_t)idx < forb_cap)
        return;
    size_t new_cap = forb_cap ? forb_cap * 2 : 64;
    while (new_cap <= (size_t)idx)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(forbidden, new_cap * sizeof(bool));
    if (!tmp)
        exit(EXIT_FAILURE);
    memset(tmp + forb_cap, 0, (new_cap - forb_cap) * sizeof(bool));
    forbidden = tmp;
    forb_cap = new_cap;
}

/* Ensure A array can store at least one more element. */
static inline void
ensure_a_capacity(void)
{
    if (a_len < a_cap)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(Aarr, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    Aarr = tmp;
    a_cap = new_cap;
}

/* Generate A values up to the (n+1)-th term (1‑based). */
static void
ensure_sequence(idx_t n)
{
    /* Need values up to index n+1 (since a(n) uses A(n) and A(n+1)). */
    while (a_len < (size_t)(n + 1)) {
        /* Find the next admissible value. */
        idx_t val = next_candidate;
        while (1) {
            ensure_forbidden(val);
            if (!forbidden[val])
                break;
            ++val;
        }

        /* Store the value. */
        ensure_a_capacity();
        Aarr[a_len] = (seq_t)val;

        /* Mark numbers forbidden by this new value. */
        idx_t f1 = val * 2 + 1;
        idx_t f2 = val * 3 + 1;
        ensure_forbidden(f1);
        ensure_forbidden(f2);
        forbidden[f1] = true;
        forbidden[f2] = true;

        /* Advance state for next iteration. */
        ++a_len;
        next_candidate = val + 1;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        /* Arrays grow lazily; no further init needed. */
    }
    if (n == 0)
        return 0;   /* undefined by offset; safe fallback */

    ensure_sequence(n);

    /* Return the difference A(n+1) - A(n) */
    seq_t diff = Aarr[(size_t)n] - Aarr[(size_t)n - 1];
    return diff;
}
