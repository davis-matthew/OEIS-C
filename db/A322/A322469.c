/*
 * A322469  Irregular table: row i = 1, 2, 3, ... starts with 4*i - 1; then,
 *          as long as the number is divisible by 3, the next two terms are
 *          the result of dividing it by 3, then multiplying it by 2.
 * https://oeis.org/A322469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the n-th element of the flattened irregular table
 *            defined by rows i=1,2,3,... where each row starts with 4*i-1
 *            and, while that value is divisible by 3, the next two terms are
 *            the result of dividing it by 3 and then multiplying that result
 *            by 2. The table is generated row‑by‑row and concat...
 * Range:     u8 n <= 132, u16 n <= 9446, u32 n <= 9446, u64 n <= 9446,
 *            u128 n <= 9446, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;      /* array of computed terms          */
static idx_t cache_len = 0;      /* number of terms currently cached */
static size_t cache_cap = 0;     /* current capacity of array         */

static idx_t gen_i = 1;          /* current row index                */
static seq_t gen_x;              /* current value in row (start or from previous step) */
static int   gen_stage = 0;      /* 0 = output start, 1 = output div, 2 = output mul */
static seq_t last_div;           /* result of last division (for multiplication)    */

/* Generate the next term of the sequence according to the row rules. */
static seq_t next_term(void)
{
    seq_t ret;
    static const seq_t three = 3;
    static const seq_t two   = 2;

    if (gen_stage == 0) {                         /* output start of row */
        ret = gen_x;
        if ((gen_x % three) == 0) {               /* divisible by 3?  */
            last_div = gen_x / three;
            gen_stage = 1;
        } else {                                   /* row finished    */
            gen_i++;
            gen_x = ((seq_t)gen_i) * 4 - ((seq_t)1);
            gen_stage = 0;
        }
        return ret;
    }

    if (gen_stage == 1) {                         /* output division   */
        ret = last_div;
        gen_stage = 2;
        return ret;
    }

    /* gen_stage == 2: output multiplication of last division    */
    ret = last_div * two;
    gen_x = ret;
    if ((gen_x % three) == 0) {                   /* continue row     */
        last_div = gen_x / three;
        gen_stage = 1;
    } else {                                       /* row finished     */
        gen_i++;
        gen_x = ((seq_t)gen_i) * 4 - ((seq_t)1);
        gen_stage = 0;
    }
    return ret;
}

/* Ensure the cache contains at least n terms (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0) {                         /* first call initialisation */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        /* initialise generator for the first row */
        gen_i = 1;
        gen_x = ((seq_t)gen_i) * 4 - ((seq_t)1);
        gen_stage = 0;
    }

    while (cache_len < n) {
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *temp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!temp) abort();
            cache    = temp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = next_term();
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offset is 1; not used for n=0 */
    ensure_cache(n);
    return cache[n - 1];
}
