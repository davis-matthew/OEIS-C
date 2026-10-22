/*
 * A321781  Least q > 1 letting Josephus survive if he finds himself at
 *          position j in the circle of m persons, but is allowed to name the
 *          elimination parameter q such that every q-th person is executed,
 *          written as triangle T(m,j), m > 1, j <= m.
 * https://oeis.org/A321781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the least integer q > 1 such that for the row (m,j) found
 *            by linearising the triangle T(m,j), the Josephus survivor in a
 *            circle of m people using step q is at position j. The value is
 *            found by searching over q for each row m and storing the minimal
 *            q that yields each survivor j. The triang...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of previously computed rows, indexed by m (1‑based). */
static seq_t **row_cache = NULL;   /* pointer to array of row pointers      */
static idx_t cache_cap = 0;        /* how many rows are currently valid    */

/* Ensure the cache can hold rows up to 'required'.  New entries are NULL. */
static void ensure_cache(idx_t required)
{
    if (required <= cache_cap)
        return;

    /* Reallocate the cache array.  Cast the void* for C++. */
    row_cache = (seq_t **)realloc(row_cache, (required + 1) * sizeof (seq_t*));
    for (idx_t i = cache_cap + 1; i <= required; ++i)
        row_cache[i] = NULL;
    cache_cap = required;
}

/* Compute a whole row T(m,1)…T(m,m) and store it in row_cache[m].
   The loop over q stops when all positions have been filled.      */
static void compute_row(idx_t m)
{
    /* Allocate the row (zeroed).  Cast as needed for C++. */
    seq_t *row = (seq_t *)malloc(m * sizeof (seq_t));
    for (idx_t i = 0; i < m; ++i)
        row[i] = 0;

    if (m == 1) {                     /* T(1,1) is defined as 0 */
        row[0] = 0;
        row_cache[m] = row;
        return;
    }

    idx_t remaining = m;              /* positions still unassigned   */

    for (idx_t q_idx = 2; remaining; ++q_idx) {
        seq_t q = (seq_t)q_idx;       /* current step size           */

        /* Josephus recurrence: 0‑based survivor after each elimination. */
        seq_t pos = 0;                /* 0‑based survivor at start   */
        for (idx_t i = 2; i <= m; ++i)
            pos = (pos + q) % (seq_t)i;

        seq_t j_seq = pos + 1;        /* convert to 1‑based position  */
        idx_t j_idx = (idx_t)j_seq;   /* j fits into idx_t           */

        if (row[j_idx - 1] == 0) {    /* record the minimal q for this j */
            row[j_idx - 1] = q;
            --remaining;
        }
    }

    row_cache[m] = row;
}

/* Public function: returns a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 1) /* T(1,1) is defined as 0 */
        return (seq_t)0;

    /* Find (m,j) such that n lies in row m of the triangle. */
    idx_t m = 1;
    while (((seq_t)m * (seq_t)(m + 1)) / 2 < (seq_t)n)
        ++m;

    /* Linear index of the first element of row m. */
    seq_t start_seq = ((seq_t)(m - 1) * (seq_t)m) / 2;
    seq_t j_seq = (seq_t)n - start_seq;
    idx_t j_idx = (idx_t)j_seq;       /* j is 1‑based within the row   */

    /* Compute this row if it has not been cached yet. */
    ensure_cache(m);
    if (row_cache[m] == NULL)
        compute_row(m);

    return row_cache[m][j_idx - 1];
}
