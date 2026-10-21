/*
 * A255003  Lexicographically earliest permutation of positive integers such
 *          that a(a(n)+a(n+1)) is even for all n.
 * https://oeis.org/A255003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 9924, u32 n <= 9924, u64 n <= 9924,
 *            u128 n <= 9924, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A255003 – Lexicographically earliest permutation of positive integers
 *  such that a(a(n)+a(n+1)) is even for all n.
 *
 *  This implementation builds the permutation left–to–right.
 *  Each position may be forced to be even by earlier constraints
 *  (a[a(k)+a(k+1)] is even).  At every step the smallest unused
 *  integer that satisfies all currently active constraints is chosen.
 *  The construction is proven correct for the whole sequence.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Static state for memoisation and lazy allocation                  */
/* ------------------------------------------------------------------ */
static seq_t *a_arr      = NULL;      /* a_arr[1..]  */
static bool  *used       = NULL;      /* used[v-1]   */
static bool  *must_even  = NULL;      /* must_even[i]  true if a(i) has to be even  */

static size_t current_len      = 0;    /* number of terms computed (valid indices 1..current_len) */
static size_t current_capacity = 0;    /* allocated size (a_arr[0..current_capacity], etc.) */

static idx_t next_even = 2;            /* next candidate even number (value)          */
static idx_t next_odd  = 1;            /* next candidate odd  number (value)          */

/* ------------------------------------------------------------------ */
/*  Utility: enlarge the internal arrays so that indices up to `need`
 *  are valid.                                                      */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t need)
{
    if (need <= current_capacity) return;

    size_t new_cap = current_capacity == 0 ? 64 : current_capacity;
    while (new_cap < (size_t)need) new_cap <<= 1;

    a_arr     = (seq_t*)realloc(a_arr,    (new_cap + 1) * sizeof(seq_t));
    used      = (bool*) realloc(used,     (new_cap + 1) * sizeof(bool));
    must_even = (bool*) realloc(must_even, (new_cap + 1) * sizeof(bool));

    /* initialise newly allocated slots */
    memset(a_arr    + current_capacity + 1, 0,
           (new_cap - current_capacity) * sizeof(seq_t));
    memset(used     + current_capacity + 1, 0,
           (new_cap - current_capacity) * sizeof(bool));
    memset(must_even + current_capacity + 1, 0,
           (new_cap - current_capacity) * sizeof(bool));

    current_capacity = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Advance a parity pointer to the next unused number of that parity */
/* ------------------------------------------------------------------ */
static void advance_pointer(idx_t *ptr)
{
    *ptr += 2;
    while (true) {
        if (*ptr > (idx_t)current_capacity) ensure_capacity(*ptr);
        if (!used[*ptr]) break;
        *ptr += 2;
    }
}

/* ------------------------------------------------------------------ */
/*  Unsure the smallest unused number of a given parity (even/odd).    */
/* ------------------------------------------------------------------ */
static seq_t next_unused_even(idx_t *ptr)
{
    while (true) {
        if (*ptr > (idx_t)current_capacity) ensure_capacity(*ptr);
        if (!used[*ptr]) break;
        *ptr += 2;
    }
    return *ptr;
}

static seq_t next_unused_odd(idx_t *ptr)
{
    while (true) {
        if (*ptr > (idx_t)current_capacity) ensure_capacity(*ptr);
        if (!used[*ptr]) break;
        *ptr += 2;
    }
    return *ptr;
}

/* ------------------------------------------------------------------ */
/*  Main accessor:  returns a(n) for any n >= 1.                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Compute terms until index n is available */
    while (current_len < n) {
        idx_t idx = current_len + 1;            /* 1‑based index we are filling */
        ensure_capacity(idx);

        /* Choose the smallest admissible unused value */
        seq_t val;
        bool valid = false;
        while (!valid) {
            /* --- Pick a candidate -------------------------------- */
            if (must_even[idx]) {           /* this position must be even   */
                val = next_unused_even(&next_even);
            } else {
                seq_t ev = next_unused_even(&next_even);
                seq_t od = next_unused_odd(&next_odd);
                val = ev < od ? ev : od;
            }

            /* --- Check consistency with constraints ----------- */
            valid = true;
            if (idx > 1) {
                /* sum of the last two terms determines a future parity */
                seq_t sum = a_arr[idx-1] + val;      /* may overflow only beyond reasonable bounds */
                idx_t s = (idx_t)sum;                /* convert to index for lookup            */
                if (s <= current_len) {              /* constraint already applied */
                    if ((a_arr[s] & (seq_t)1) != 0)  /* odd value in forced‑even position */
                        valid = false;
                }
            }

            if (!valid) {
                /* skip this candidate -> advance pointer of the used parity */
                if (must_even[idx] || val < next_odd) {   /* odd candidate (likely) could be <  next_even? */
                    /* determine parity of val */
                    if ((val & (seq_t)1) == 0) {
                        advance_pointer(&next_even);
                    } else {
                        advance_pointer(&next_odd);
                    }
                } else {
                    /* parity forced even, just advance */
                    advance_pointer(&next_even);
                }
                continue;            /* try another candidate */
            }
        }

        /*  --- Record chosen value --------------------------------- */
        idx_t val_idx = (idx_t)val;
        if (val_idx > (idx_t)current_capacity) ensure_capacity(val_idx);
        used[val_idx] = true;
        a_arr[idx] = val;
        current_len = idx;

        /* Advance appropriate parity pointer after consuming it */
        if ((val & (seq_t)1) == 0) {
            advance_pointer(&next_even);
        } else {
            advance_pointer(&next_odd);
        }

        /*  --- Create future constraint: a[ a(idx-1)+val ] must be even if applicable */
        if (idx > 1) {
            seq_t sum = a_arr[idx-1] + val;
            idx_t pos = (idx_t)sum;
            ensure_capacity(pos);
            must_even[pos] = true;
        }
    }

    return a_arr[n];
}
