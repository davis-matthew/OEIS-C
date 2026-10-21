/*
 * A256173  Numbers k such that ceiling(sqrt(k))^2 - k is a square.
 * https://oeis.org/A256173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that ceil(sqrt(k))^2 – k is a square
 * Range:     u8 n <= 67, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit value, without overflow. */
/* Returns floor(sqrt(n)). */
static inline idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t low = 1, high = n >> 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            if ((mid + 1) > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
    }
    return 0;
}

/* -----  Cached generation of the sequence  ----- */

static seq_t *terms = NULL;       /* sorted ascending */
static size_t terms_cap   = 0;    /* allocated capacity */
static size_t terms_size  = 0;    /* number of stored terms  */
static idx_t  current_m   = 0;    /* last m processed      */

/* Ensure the internal buffer can hold at least `need` elements. */
static void ensure_capacity(size_t need) {
    if (terms_cap < need) {
        size_t new_cap = terms_cap == 0 ? 64 : terms_cap;
        while (new_cap < need) new_cap <<= 1;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* Main function required by the build. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;           /* undefined, but safe */

    /* Initialise the cached data on first use. */
    if (terms_size == 0) {
        ensure_capacity(64);
        terms[0] = (seq_t)0;              /* k = 0 */
        terms_size = 1;
        current_m = 0;
    }

    /* Generate more terms until we have at least n. */
    while (terms_size < n) {
        /* Next m to process. */
        idx_t m = ++current_m;            /* m = 1,2,3,... */

        /* Upper bound for t.  t^2 < 2m – 1  →  t <= floor(sqrt(2m-2)). */
        idx_t tmax = isqrt_u64(2 * m - 2);

        /* Produce k = m^2 – t^2 for t = tmax down to 0 (ascending k). */
        for (idx_t t = tmax; ; --t) {
            seq_t km = (seq_t)m * (seq_t)m;
            seq_t tk = (seq_t)t * (seq_t)t;
            seq_t k   = km - tk;

            /* k must be strictly greater than (m-1)^2; this tests the
               interval ( (m-1)^2 , m^2 ]. */
            if (k > (seq_t)(m - 1) * (seq_t)(m - 1)) {
                ensure_capacity(terms_size + 1);
                terms[terms_size++] = k;
            }

            if (t == 0) break;             /* loop over all t */
        }
    }

    return terms[n - 1];
}
