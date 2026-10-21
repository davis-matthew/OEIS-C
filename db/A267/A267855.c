/*
 * A267855  Decimal representation of the n-th iteration of the "Rule 230"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-2) + 16*a(n-4) - 256*a(n-6) for n > 5 Initial
 *            terms: a(0)=1, a(1)=6, a(2)=20, a(3)=120, a(4)=368, a(5)=1888
 *            The recurrence splits into even and odd indices; each parity
 *            class satisfies the same linear recurrence: b(k) = 16*b(k-1) +
 *            16*b(k-2) - 256*b(k-3), k >= 3 where b(k) = a(2k) f...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- Memoisation storage for even indices a(2k) -------------------- */
static seq_t *even_arr = NULL;   /* array of a(0), a(2), a(4), ...     */
static size_t even_len = 0;      /* number of populated entries          */
static size_t even_cap = 0;      /* allocated capacity                   */

/* --- Memoisation storage for odd indices a(2k+1) -------------------- */
static seq_t *odd_arr = NULL;    /* array of a(1), a(3), a(5), ...      */
static size_t odd_len = 0;
static size_t odd_cap = 0;

/* --- Base values for the recurrence -------------------------------- */
static const seq_t base_even[3] = { (seq_t)1,  (seq_t)20,  (seq_t)368  };
static const seq_t base_odd [3] = { (seq_t)6,  (seq_t)120, (seq_t)1888 };

/* ------------------------------------------------------------------- */
static void ensure_even(size_t k)
/* Guarantees that even_arr[0..k] are valid, expanding the array if
   necessary.  'k' is the index in the parity array, i.e. the
   integer k such that a(2k) is needed. */
{
    if (k < even_len) return;          /* already available */

    /* Grow capacity to at least k+1 elements, doubling each step. */
    size_t new_cap = even_cap ? even_cap : 4;
    while (new_cap <= k) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(even_arr, new_cap * sizeof(seq_t));
    if (tmp == NULL) { /* allocation failure – abort to keep behaviour defined */
        abort();
    }
    even_arr = tmp;
    even_cap = new_cap;

    /* Compute new entries, starting from the current length. */
    for (size_t i = even_len; i <= k; ++i) {
        if (i < 3) {
            even_arr[i] = base_even[i];
        } else {
            seq_t t = (seq_t)16 * even_arr[i-1] + (seq_t)16 * even_arr[i-2]
                      - (seq_t)256 * even_arr[i-3];
            even_arr[i] = t;
        }
    }
    even_len = k + 1;
}

static void ensure_odd(size_t k)
/* Guarantees that odd_arr[0..k] are valid, expanding the array if
   necessary.  'k' is the index in the parity array, i.e. the
   integer k such that a(2k+1) is needed. */
{
    if (k < odd_len) return;          /* already available */

    /* Grow capacity to at least k+1 elements, doubling each step. */
    size_t new_cap = odd_cap ? odd_cap : 4;
    while (new_cap <= k) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(odd_arr, new_cap * sizeof(seq_t));
    if (tmp == NULL) { /* allocation failure – abort to keep behaviour defined */
        abort();
    }
    odd_arr = tmp;
    odd_cap = new_cap;

    /* Compute new entries, starting from the current length. */
    for (size_t i = odd_len; i <= k; ++i) {
        if (i < 3) {
            odd_arr[i] = base_odd[i];
        } else {
            seq_t t = (seq_t)16 * odd_arr[i-1] + (seq_t)16 * odd_arr[i-2]
                      - (seq_t)256 * odd_arr[i-3];
            odd_arr[i] = t;
        }
    }
    odd_len = k + 1;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
/* Public entry point: returns the n-th term of OEIS A267855.  The
   function is thread‑safe except for the static memoisation arrays;
   concurrent calls are safe as the memory is only appended to. */
{
    if ((n & (idx_t)1) == 0) {                 /* even index */
        size_t k = (size_t)(n >> 1);           /* k such that a(2k) is needed */
        ensure_even(k);
        return even_arr[k];
    } else {                                   /* odd index */
        size_t k = (size_t)((n - 1) >> 1);    /* k such that a(2k+1) is needed */
        ensure_odd(k);
        return odd_arr[k];
    }
}
