/*
 * A339637  Numbers congruent to 1 (mod 3) that are the quotient of two Cantor
 *          numbers (A005823).
 * https://oeis.org/A339637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A339637: Numbers ≡ 1 (mod 3) that are quotients of two Cantor numbers.
    A number n appears iff it is not contained in any interval
    ((3/2)*3^i, 2*3^i) for some integer i ≥ 0.  This routine
    generates the sequence by scanning the 1‑mod‑3 integers, skipping
    the forbidden intervals, and storing the accepted numbers in a
    growable cache.  The n‑th term is returned (1‑based). */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* memoised values */
static size_t cap   = 0;      /* current capacity of cache */
static size_t len   = 0;      /* number of terms stored */
static seq_t tested_m = 1;    /* next candidate m ≡ 1 (mod 3) to test */

/*--- Helper ---------------------------------------------------------------*/
/* Return true iff m lies in some forbidden interval ( (3/2)*3^i , 2*3^i ). */
static bool is_forbidden(seq_t m)
{
    /* Find the greatest power of 3 not exceeding m. */
    seq_t pow = 1;
    while (pow <= m / 3)             /* integer arithmetic */
        pow *= 3;                   /* now pow == 3^i ,  pow <= m < 3*pow */

    /* Check  m > 1.5*pow  and  m < 2*pow  <=> 2*m > 3*pow  and  2*m < 4*pow */
    seq_t twice      = m * 2;
    seq_t three_pow  = pow * 3;
    seq_t four_pow   = pow * 4;

    return (twice > three_pow) && (twice < four_pow);
}

/*--- Public function -------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n==0; return 0 as sentinel */
        return 0;

    /* Initialise cache on first use */
    if (cache == NULL) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        seq_t m = tested_m;
        tested_m += 3;                 /* next 1 (mod 3) candidate */

        if (!is_forbidden(m)) {        /* accept the candidate */
            if (len == cap) {          /* grow cache if needed */
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (tmp == NULL) {
                    abort();           /* allocation failed */
                }
                cache = tmp;
            }
            cache[len++] = m;           /* store accepted term */
        }
    }

    return cache[n-1];                 /* a(1) is first element */
}
