/*
 * A242522  Number of cyclic arrangements of S={1,2,...,n} such that the
 *          difference between any two neighbors is at least 2.
 * https://oeis.org/A242522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n a(n-1) - (n-5) a(n-2) - (n-4) a(n-3) + (n-4) a(n-4)
 *            (n>6) a(1)=a(2)=a(3)=a(4)=0, a(5)=1, a(6)=5
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;       /* cached values */
static idx_t memo_size = 0;      /* largest n computed */
static idx_t memo_cap  = 0;      /* allocated capacity */

/* Ensure the memo table can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= memo_cap)
        return;
    idx_t new_cap = memo_cap ? memo_cap : 8;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_mem) exit(1);
    if (memo) {
        for (idx_t i = 0; i <= memo_size; ++i)
            new_mem[i] = memo[i];
        free(memo);
    }
    memo = new_mem;
    memo_cap = new_cap;
}

/* Initialise base terms a(1)...a(6). */
static void init_base(void)
{
    ensure_capacity(6);
    memo[1] = 0;
    memo[2] = 0;
    memo[3] = 0;
    memo[4] = 0;
    memo[5] = 1;
    memo[6] = 5;
    memo_size = 6;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (memo_size == 0)
        init_base();
    if (n <= memo_size)
        return memo[n];

    /* Compute all terms up to n using the recurrence. */
    ensure_capacity(n);
    for (idx_t i = memo_size + 1; i <= n; ++i) {
        /* positive part: n*a(i-1) + (i-4)*a(i-4) */
        seq_t pos = (seq_t)i * memo[i - 1] + (seq_t)(i - 4) * memo[i - 4];
        /* negative part: (i-5)*a(i-2) + (i-4)*a(i-3) */
        seq_t neg = (seq_t)(i - 5) * memo[i - 2] + (seq_t)(i - 4) * memo[i - 3];
        memo[i] = pos - neg;   /* pos >= neg for i>6, so no wrap */
    }
    memo_size = n;
    return memo[n];
}
