/*
 * A297352  a(n) is the smallest positive number not yet in the sequence that
 *          if n is even, contains the smallest digit in a(n-1), and if n is
 *          odd, contains the largest digit in a(n-1); a(1)=0.
 * https://oeis.org/A297352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 247, u16 n <= 9886, u32 n <= 9886, u64 n <= 9886,
 *            u128 n <= 9886, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=0; for n≥2, a(n) is the smallest positive integer not yet in the sequence that
   contains the smallest digit of a(n−1) when n is even, and the largest digit of a(n−1)
   when n is odd. */

/* Standard headers – needed for malloc, size_t and bool */
#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoisation */
static seq_t *seq = NULL;              /* array of computed terms */
static size_t seq_cap = 0;             /* allocated capacity of seq */
static idx_t max_n_computed = 0;       /* number of terms stored */

/* Used flag array for quick membership testing.
   It is resized as needed. */
static bool *used = NULL;              /* used[v] == true if v already appeared */
static size_t used_cap = 0;             /* current capacity */

/* array of next candidate for each digit 0‑9
   next_candidate[d] holds the smallest integer ≥ 0 that has digit d
   and has not yet been chosen for a term with that target digit. */
static seq_t next_candidate[10];

/* ------------------------------------------------------------------ */
/* Utility: smallest decimal digit of x, treating 0 as a valid digit */
static inline int smallest_digit(seq_t x)
{
    if (x == (seq_t)0) return 0;
    int best = 9;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        if (d < best) {
            best = d;
            if (best == 0) break;
        }
        x /= (seq_t)10;
    }
    return best;
}

/* Utility: largest decimal digit of x, treating 0 as a valid digit */
static inline int largest_digit(seq_t x)
{
    if (x == (seq_t)0) return 0;
    int best = 0;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        if (d > best) {
            best = d;
            if (best == 9) break;
        }
        x /= (seq_t)10;
    }
    return best;
}

/* Does x contain decimal digit d? */
static inline bool contains_digit(seq_t x, int d)
{
    if (x == (seq_t)0) return d == 0;
    while (x > (seq_t)0) {
        if ((int)(x % (seq_t)10) == d) return true;
        x /= (seq_t)10;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Auxiliary: make the used array large enough to index value v.
   We assume that v fits in size_t; for all practical values in this
   sequence it does. */
static inline void ensure_used_capacity(size_t v)
{
    if (v < used_cap) return;
    size_t new_cap = used_cap == 0 ? 64 : used_cap * 2;
    while (new_cap <= v) new_cap *= 2;
    bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_used) abort();          /* allocation failure */
    /* initialise new slots to false */
    for (size_t i = used_cap; i < new_cap; ++i) new_used[i] = false;
    used = new_used;
    used_cap = new_cap;
}

/* Check if value v has already appeared in the sequence */
static inline bool seen(seq_t v)
{
    size_t idx = (size_t) v;
    ensure_used_capacity(idx);
    return used[idx];
}

/* Record that value v has just appeared */
static inline void mark_used(seq_t v)
{
    size_t idx = (size_t) v;
    ensure_used_capacity(idx);
    used[idx] = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not part of the sequence but harmless */

    /* initialise on first call */
    if (max_n_computed == 0) {
        seq_cap = 128;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();          /* allocation failure */

        used = NULL;
        used_cap = 0;

        /* a(1) = 0 */
        seq[0] = (seq_t)0;
        mark_used((seq_t)0);
        max_n_computed = 1;

        /* initialise next_candidate for each digit to 0 */
        for (int d = 0; d < 10; ++d) next_candidate[d] = (seq_t)0;
    }

    /* grow array if needed (n is 1‑based) */
    if (n > (idx_t)seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }

    /* if already computed, just return it */
    if (n <= max_n_computed) return seq[n - 1];

    /* generate missing terms up to n */
    for (idx_t k = max_n_computed + 1; k <= n; ++k) {
        seq_t prev = seq[k - 2];                   /* a(k‑1)   */
        int target = (k % 2 == 0) ? smallest_digit(prev)
                                  : largest_digit(prev);

        seq_t cand = next_candidate[target];
        /* Find the next unused number that contains the target digit */
        while (true) {
            if (contains_digit(cand, target) && !seen(cand)) break;
            ++cand;
        }

        seq[k - 1] = cand;
        mark_used(cand);
        /* Advance the candidate starting point for this digit */
        next_candidate[target] = cand + (seq_t)1;
    }

    max_n_computed = n;
    return seq[n - 1];
}
