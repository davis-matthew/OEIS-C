/*
 * A257224  Numbers that have at least one divisor containing the digit 7 in
 *          base 10.
 * https://oeis.org/A257224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257224: numbers that have at least one divisor containing the digit 7.
   Implementation searches integers incrementally, storing those that satisfy the
   property. This method works for all supported seq_t types, including
   arbitrary‑precision integers. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
/* In C++ allocate with new[] and delete[] to allow construction of complex types. */
#define ALLOC_SEQ_TARRAY(cap) (new seq_t[(cap)])
#define FREE_SEQ_TARRAY(ptr) delete [] (ptr)
#else
/* In C use malloc/free. */
#define ALLOC_SEQ_TARRAY(cap) ((seq_t*)malloc((cap)*sizeof(seq_t)))
#define FREE_SEQ_TARRAY(ptr) free(ptr)
#endif

/* ------------------------------------------------------------------------ */
/* Helper: does x contain the digit 7? */
static bool has_digit7(idx_t x)
{
    if (x == 0) return false;
    while (x > 0) {
        if (x % 10 == 7) return true;
        x /= 10;
    }
    return false;
}

/* ------------------------------------------------------------------------ */
/* Static storage for the computed sequence */
static seq_t *seq_vec     = NULL;   /* array of sequence elements              */
static size_t seq_capacity = 0;     /* allocated capacity                       */
static size_t seq_len      = 0;     /* number of stored terms                   */
static idx_t   next_candidate = 1;  /* next integer to test                      */

/* ------------------------------------------------------------------------ */
/* Ensure the vector has room for at least one more element */
static void seq_grow(void)
{
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 8;
    seq_t *new_vec = ALLOC_SEQ_TARRAY(new_cap);
    if (!new_vec) return; /* allocation failure – we simply abort silently */

    for (size_t i = 0; i < seq_len; ++i) new_vec[i] = seq_vec[i];
    FREE_SEQ_TARRAY(seq_vec);
    seq_vec = new_vec;
    seq_capacity = new_cap;
}

/* ------------------------------------------------------------------------ */
/* Push a new term into the sequence */
static void seq_push(seq_t val)
{
    if (seq_len == seq_capacity) seq_grow();
    seq_vec[seq_len++] = val;
}

/* ------------------------------------------------------------------------ */
/* Main function exposed to callers */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        idx_t cand = next_candidate;

        bool ok = false;

        /* If the candidate itself has a 7, it qualifies immediately. */
        if (has_digit7(cand)) ok = true;
        else {
            /* Search its divisors */
            for (idx_t d = 1; d * d <= cand; ++d) {
                if (cand % d == 0) {
                    if (has_digit7(d) ||
                        (d != cand / d && has_digit7(cand / d))) {
                        ok = true;
                        break;
                    }
                }
            }
        }

        if (ok) seq_push((seq_t)cand);

        /* Advance candidate for next iteration */
        next_candidate = cand + 1;
    }

    return seq_vec[n - 1];
}
