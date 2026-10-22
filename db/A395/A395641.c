/*
 * A395641  Triangular array: row n shows the coefficients of the polynomial -
 *          n!*L(n, x), where L(n, x) is the Lagrange interpolating polynomial
 *          that passes through (0,-1) and (k,1) for k = 1..n.
 * https://oeis.org/A395641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = coefficient of x^k in P_r(x) = 2*(-1)^r ∏_{i=1}^{r}(x-i)
 *            - r!, where r = floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2
 * Range:     i8 n <= 15, i16 n <= 35, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Routine to compute the OEIS sequence A395641. */
#include <stdlib.h>
#include <stddef.h>

/* static storage for values computed so far */
static seq_t *flat = NULL;          /* flattened coefficients */
static size_t flat_len = 0;         /* number of terms stored */
static size_t flat_cap = 0;         /* allocated capacity of flat */

static seq_t *fact = NULL;          /* factorials fac[i] = i! */
static size_t fact_cap = 0;         /* capacity for fact array */

static seq_t *b = NULL;            /* coefficients of B_r(x) = ∏_{j=1}^{r}(x-j) */
static size_t b_cap = 0;           /* capacity of b array */

static idx_t cur_row = 0;           /* last fully processed row */

/* Grow a seq_t array to at least 'needed' elements. */
static void ensure_capacity(seq_t **ptr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap ? (*cap << 1) : 16);
    if (new_cap < needed) new_cap = needed;
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Initialise row 0 (constant term 1). */
static void init_row0(void)
{
    if (!flat) {
        /* create storages with minimal sizes */
        ensure_capacity(&flat, &flat_cap, 16);
        ensure_capacity(&fact, &fact_cap, 16);
        ensure_capacity(&b,   &b_cap,   16);

        /* factorial[0] = 1 */
        fact[0] = (seq_t)1;

        /* B_0(x) = 1 -> coefficient array b[0] = 1 */
        b[0] = (seq_t)1;

        /* row 0: only one coefficient = factorial[0] */
        flat[0] = fact[0];
        flat_len = 1;

        cur_row = 0;
    }
}

/* Compute the next row r = cur_row + 1. */
static void compute_next_row(void)
{
    idx_t r = cur_row + 1;

    /* factorial of r */
    ensure_capacity(&fact, &fact_cap, r + 1);
    fact[r] = fact[r - 1] * (seq_t)r;

    /* grow b array */
    ensure_capacity(&b, &b_cap, r + 1);

    /* Update B_r(x) = (x-r) * B_{r-1}(x) in place, from high to low. */
    for (idx_t k = r; ; --k) {
        seq_t new_val;
        if (k == r) {
            new_val = b[r - 1];
        } else if (k == 0) {
            new_val = -(b[0] * (seq_t)r);
        } else {
            new_val = b[k - 1] - (b[k] * (seq_t)r);
        }
        b[k] = new_val;
        if (k == 0) break;
    }

    /* Append coefficients of P_r(x) to the flat array. */
    ensure_capacity(&flat, &flat_cap, flat_len + (r + 1));
    seq_t sign_factor = (r & 1) ? (seq_t)-2 : (seq_t)2;  /* 2*(-1)^r */
    for (idx_t k = 0; k <= r; ++k) {
        seq_t value;
        if (k == 0)
            value = fact[r];           /* constant term n! */
        else
            value = sign_factor * b[k];
        flat[flat_len++] = value;
    }

    cur_row = r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_row0();                     /* ensure row 0 is ready */

    /* Compute rows until the requested term is available. */
    while (flat_len <= n) compute_next_row();

    return flat[n];
}
