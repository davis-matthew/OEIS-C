/*
 * A266111  If A082284(n) = 0, a(n) = 1, otherwise a(n) = 1 + a(A082284(n)),
 *          where A082284(n) = smallest number k such that k - d(k) = n, or 0
 *          if no such number exists, and d(n) = the number of divisors of n
 *          (A000005).
 * https://oeis.org/A266111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1 if A082284(n)=0; else 1 + a(A082284(n))
   where A082284(n) is the smallest k with k - d(k) = n
   (d(k) = number of divisors of k).  The function memoises results.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

#define MAX_N 200000ULL   /* sufficient for n <= 10000 */

static idx_t LIMIT = MAX_N;
static idx_t *DIVCNT = NULL;   /* divisor counts up to LIMIT */
static idx_t *PREIMG = NULL;   /* smallest k with k - d(k) = n */
static seq_t *ACACHE = NULL;   /* memoised a(n) values */
static int  _initialized = 0;

/* ------------------------------------------------------------------ */
/* initialise the tables only once                                   */
static void init_tables(void)
{
    if (_initialized) return;
    _initialized = 1;

    /* allocate the arrays */
    DIVCNT = (idx_t*)malloc( (LIMIT + 1) * sizeof(idx_t) );
    PREIMG = (idx_t*)malloc( (LIMIT + 1) * sizeof(idx_t) );
    ACACHE = seq_alloc(LIMIT + 1);          /* returns zero‑initialised array */

    if (!DIVCNT || !PREIMG || !ACACHE) {
        /* allocation failed – abort */
        exit(1);
    }

    /* divisor counts */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            ++DIVCNT[j];
        }
    }

    /* pre‑image mapping (first occurrence gives the smallest k) */
    for (idx_t k = 1; k <= LIMIT; ++k) {
        idx_t m = k - DIVCNT[k];
        if (m <= LIMIT && PREIMG[m] == 0) {
            PREIMG[m] = k;
        }
    }
}

/* ------------------------------------------------------------------ */
/* sequence a(n)                                                       */
seq_t a(idx_t n)
{
    init_tables();                /* ensure tables are ready */

    /* fast return if already computed */
    if (ACACHE[n] != 0) return ACACHE[n];

    /* iterative evaluation to avoid recursion limits */
    idx_t cur = n;
    seq_t count = 1;              /* a(n) is at least 1 */

    while (PREIMG[cur] != 0) {
        cur = PREIMG[cur];
        ++count;
    }

    ACACHE[n] = count;            /* memoise */
    return count;
}
