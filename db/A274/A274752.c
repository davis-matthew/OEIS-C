/*
 * A274752  Number of 5Xn 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (0,-1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition (empirical relation for n>17)
   a(n) = 2*a(n-1) + 55*a(n-2) + 38*a(n-3) -614*a(n-4)
          -600*a(n-5) +2769*a(n-6) +2140*a(n-7)
          -6482*a(n-8) -2606*a(n-9) +7803*a(n-10)
          +678*a(n-11) -4266*a(n-12) +304*a(n-13)
          +924*a(n-14) -72*a(n-15) -72*a(n-16)   (n>17)
   Initial values: a(1)=8, a(2)=81, a(3)=610, a(4)=4967, a(5)=40242,
   a(6)=327123, a(7)=2661918, a(8)=21656459, a(9)=176259458,
   a(10)=1434313078, a(11)=11673115263, a(12)=94995850464,
   a(13)=773101634059, a(14)=6291606309132, a(15)=51202379223709,
   a(16)=416693699123591, a(17)=3391131540449104. */

#include <stdlib.h>

static const seq_t init[17] = {
    (seq_t)8ULL,
    (seq_t)81ULL,
    (seq_t)610ULL,
    (seq_t)4967ULL,
    (seq_t)40242ULL,
    (seq_t)327123ULL,
    (seq_t)2661918ULL,
    (seq_t)21656459ULL,
    (seq_t)176259458ULL,
    (seq_t)1434313078ULL,
    (seq_t)11673115263ULL,
    (seq_t)94995850464ULL,
    (seq_t)773101634059ULL,
    (seq_t)6291606309132ULL,
    (seq_t)51202379223709ULL,
    (seq_t)416693699123591ULL,
    (seq_t)3391131540449104ULL
};

static seq_t *memo      = NULL;
static size_t memo_cap  = 0;   /* allocated size (indices 0..memo_cap-1)   */
static idx_t  memo_max  = 0;   /* highest index whose value is defined    */

static const int coeff[16] = {
    2, 55, 38, -614, -600, 2769, 2140,
   -6482, -2606, 7803, 678, -4266, 304,
    924, -72, -72
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Ensure we have storage up to max(n,17) */
    size_t need = (size_t)n + 1;
    if (need < 18) need = 18;           /* need room for initial terms      */

    if (memo_cap < need) {
        size_t new_cap = need;
        seq_t *tmp = NULL;

        if (memo == NULL)
            tmp = (seq_t*)calloc(new_cap, sizeof(seq_t));
        else
            tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));

        if (!tmp) exit(EXIT_FAILURE);  /* allocation error */

        /* Initialise first 17 entries if they were not already */
        if (memo_cap < 18) {
            for (int i = 1; i <= 17; ++i)
                tmp[i] = init[i-1];
        }
        memo     = tmp;
        memo_cap = new_cap;
    }

    /* We guarantee that a(1)..a(17) are already set   */
    if (memo_max < 17) memo_max = 17;

    /* If the requested term is already computed return it */
    if (n <= memo_max) return memo[n];

    /* Compute missing terms up to n */
    for (idx_t i = memo_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (int j = 0; j < 16; ++j) {
            seq_t term = memo[i-1-j];
            seq_t prod = term * (seq_t)(coeff[j] >= 0 ? coeff[j] : -coeff[j]);
            if (coeff[j] >= 0)
                sum += prod;
            else
                sum -= prod;
        }
        memo[i] = sum;
    }
    memo_max = n;
    return memo[n];
}
