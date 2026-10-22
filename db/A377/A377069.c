/*
 * A377069  Triangle read by rows: T(n,k) is the number of (k+1)-vertex
 *          dominating sets of the (n+1)-path graph that include the first
 *          vertex.
 * https://oeis.org/A377069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   decompose_flattened
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A377069 – Flattened triangle T(n,k) of dominating‑set counts
 *
 *  Triangle T(n,k) defined by the generating function
 *        (1 + x) / (1 - y*x - y*x^2 - y*x^3)
 *
 *  Recurrence for n ≥ 2 and 1 ≤ k ≤ n:
 *        T(n,k) = T(n-1,k-1) + (k ≤ n-1 ? T(n-2,k-1) : 0) + (k ≤ n-2 ? T(n-3,k-1) : 0)
 *
 *  Initial values:
 *        T(0,0) = 1
 *        T(1,0) = 1
 *        T(n,0) = 0   for n ≥ 2
 *
 *  The flattened sequence a(n) consists of the rows of T read left‑to‑right
 *  and then top‑to‑bottom:  row 0, row 1, row 2, …
 *
 *  The function a(idx_t n) below returns the nth term (0‑indexed).
 *  It memoises the triangle rows in a static cache so that repeated
 *  calls are efficient and independent of call order.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */
#include "../utils/decompose_flattened.h"

/*  seq_t and idx_t are defined by the build system.  Do not re‑define them.  */
extern seq_t a(idx_t n);   /* forward declaration for the build system */

/* static cache for the computed triangle rows */
static seq_t **T        = NULL;  /* array of pointers to rows */
static size_t max_cached = 0;    /* number of rows currently cached */

/*  Ensure that rows 0 .. upto (inclusive) are cached.   */
static void ensure_rows(size_t upto)
{
    if (upto < max_cached)
        return;

    /* grow the row pointer array */
    T = (seq_t **)realloc(T, (upto + 1) * sizeof(seq_t *));
    if (T == NULL) exit(1);   /* allocation failed */

    for (size_t r = max_cached; r <= upto; ++r) {
        /* allocate the row: r+1 entries (k = 0 .. r) */
        T[r] = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (T[r] == NULL) exit(1);

        for (size_t k = 0; k <= r; ++k) {
            seq_t val = 0;

            if (k == 0) {                /* first column */
                if (r <= 1)
                    val = 1;             /* T(0,0)=1 , T(1,0)=1  */
                else
                    val = 0;             /* T(n,0)=0 for n≥2   */
            } else {                     /* k ≥ 1 */
                /* T(r-1,k-1) always exists because r-1 ≥ k-1 */
                val += T[r-1][k-1];
                /* T(r-2,k-1) exists iff k ≤ r-1 */
                if (r >= 2 && k <= r-1)
                    val += T[r-2][k-1];
                /* T(r-3,k-1) exists iff k ≤ r-2 */
                if (r >= 3 && k <= r-2)
                    val += T[r-3][k-1];
            }

            T[r][k] = val;
        }
    }
    max_cached = upto + 1;
}

/*  Public function:  a(n) – flattened triangle  */
seq_t a(idx_t n)
{
    size_t r, k;
    decompose_flattened(n, &r, &k);

    ensure_rows(r);

    return T[r][k];
}
