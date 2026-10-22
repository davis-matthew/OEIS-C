/*
 * A302513  Number of nX6 0..1 arrays with every element equal to 0, 1, 4 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-3) + 5*a(n-4) + 3*a(n-5) - 8*a(n-6) -
 *            6*a(n-7) - 3*a(n-8) (for n > 12) Initial values: a(1)=13,
 *            a(2)=13, a(3)=52, a(4)=80, a(5)=199, a(6)=458, a(7)=1088,
 *            a(8)=2651, a(9)=6257, a(10)=14765, a(11)=35292, a(12)=84261
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[12] = {
    (seq_t)13,
    (seq_t)13,
    (seq_t)52,
    (seq_t)80,
    (seq_t)199,
    (seq_t)458,
    (seq_t)1088,
    (seq_t)2651,
    (seq_t)6257,
    (seq_t)14765,
    (seq_t)35292,
    (seq_t)84261
};

static seq_t *memo = NULL;
static idx_t memo_size = 0;     /* allocated size (number of slots)   */
static idx_t last_computed = 0; /* highest index that has been filled */

/* ensures that memo has at least (n+1) slots and initial values are set */
static void ensure_cache(idx_t n)
{
    if (n >= memo_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!new_mem) {
            free(memo);
            exit(EXIT_FAILURE);
        }
        memo = new_mem;
        memo_size = (idx_t)new_size;
    }
    if (last_computed < 12) {
        /* fill first 12 entries if not already present */
        for (idx_t i = last_computed + 1; i <= 12 && i <= n; ++i) {
            memo[i] = init_vals[i - 1];
        }
        if (last_computed < 12) last_computed = 12;
    }
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but safe */

    if (n <= 12) return init_vals[n - 1];

    ensure_cache(n);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = memo[i - 1];
        val += memo[i - 3] * (seq_t)6;
        val += memo[i - 4] * (seq_t)5;
        val += memo[i - 5] * (seq_t)3;
        val -= memo[i - 6] * (seq_t)8;
        val -= memo[i - 7] * (seq_t)6;
        val -= memo[i - 8] * (seq_t)3;
        memo[i] = val;
    }
    last_computed = (last_computed > n) ? last_computed : n;
    return memo[n];
}
