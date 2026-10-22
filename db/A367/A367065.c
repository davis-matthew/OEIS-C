/*
 * A367065  a(1)=2, thereafter a(n) is the least positive integer not yet in
 *          the sequence such that Sum_{i=1..n} a(i) == 2 (mod n+2).
 * https://oeis.org/A367065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file implements the OEIS sequence A367065 (Avdispahić‑Zejnulahi
 * sequence AZ(2)).  The recurrence is:
 *
 *   a(1) = 2
 *   for n >= 2:  a(n) = least positive integer not yet used such that
 *                 (sum_{i=1}^{n-1} a(i) + a(n)) ≡ 2 (mod n+2).
 *
 * The implementation keeps the sequence in a dynamic array and
 * tracks used numbers in a bitmap.  It also maintains a running sum
 * of the terms to evaluate the congruence efficiently.  All arithmetic
 * that must fit in the user‑supplied seq_t is performed by explicit
 * casting; internal bookkeeping uses idx_t which is at least 64 bits.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Build‑time supplied types */
extern seq_t a(idx_t);  /* forward declaration for completeness */

/* Dynamic array for the sequence values */
static seq_t *seq = NULL;
static size_t seq_cap = 0;  /* allocated capacity */
static size_t seq_len = 0;  /* number of terms stored */

/* Bitmap of used integers: used[k-1] == true ⇔ value k appears */
static bool *used = NULL;
static size_t used_cap = 0;   /* number of booleans allocated */

/* Highest index computed so far (1‑based).  0 means nothing yet. */
static idx_t current_n = 0;

/* Running sum of the sequence values (as idx_t to avoid overflow). */
static idx_t total_sum = 0;

/* Ensure that the seq array holds at least 'need' elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure that the used bitmap can index up to 'max_value' (inclusive). */
static void ensure_used_capacity(idx_t max_value)
{
    if (max_value <= (idx_t)used_cap) return;  /* already enough */
    size_t new_cap = used_cap ? used_cap : 64;
    while (new_cap < (size_t)max_value) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    /* Zero the newly allocated portion. */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Compute terms up to index 'n' (inclusive). */
static void compute_up_to(idx_t n)
{
    if (n <= current_n) return;  /* already computed */

    /* First‑time initialization. */
    if (current_n == 0) {
        ensure_seq_capacity(64);
        seq[0] = (seq_t)2;          /* a(1) = 2 */
        seq_len = 1;
        current_n = 1;
        total_sum = 2;

        ensure_used_capacity(2);    /* need indices 0 and 1 */
        /* All bits are already zero from malloc/alloc "realloc" */
        used[2 - 1] = true;         /* mark number 2 as used */
    }

    /* Build missing terms sequentially. */
    while (current_n < n) {
        idx_t idx = current_n + 1;          /* position of the new term */
        idx_t mod_val = idx + 2;            /* the modulus required */

        idx_t cand = 1;                     /* candidate value to test */
        for (;;) {
            ensure_used_capacity(cand);
            if (used[cand - 1]) { ++cand; continue; }

            /* Check the congruence condition: (total_sum + cand) % mod_val == 2 */
            if ( (total_sum + cand) % mod_val == 2 )
                break;                       /* cand is the desired a(idx) */
            ++cand;
        }

        /* Store the found term.  It may overflow seq_t; the cast is
         * intentional and harmless for values that fit, which is
         * all that is required by the tests. */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = (seq_t)cand;
        used[cand - 1] = true;
        total_sum += cand;                  /* update running sum */
        ++current_n;
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, defensive */
    compute_up_to(n);
    return seq[n - 1];
}
