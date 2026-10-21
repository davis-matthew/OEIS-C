/*
 * A268903  Number of nX7 0..2 arrays with some element plus some horizontally
 *          or antidiagonally adjacent neighbor totalling two exactly once.
 * https://oeis.org/A268903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A268903 : Number of n×7 0..2 arrays with exactly one adjacent pair
   (horizontal or anti‑diagonal) summing to 2.  The values satisfy the
   empirical recurrence
     a(n)=84 a(n‑1) –2466 a(n‑2)+31428 a(n‑3) –206469 a(n‑4)
          +750384 a(n‑5) –1513404 a(n‑6)+1574640 a(n‑7) –656100 a(n‑8)
   for n≥9.  The first eight terms are obtained by a transfer‑matrix DP
   described below.  The function memoises results so that calls with
   arbitrary n (not necessarily monotonic) are efficient. */

#include <stdlib.h>
#include <stdint.h>

/* -----  Computation of the first eight terms by DP  ----- */
static void compute_initial_terms(seq_t init[8])
{
    const size_t ST = 2187;             /* 3^7 */
    uint8_t digits[ST][7];
    int horiz[ST];

    /* Precompute the base‑3 digits of each row pattern and its horizontal
       pair count (within the row).                                    */
    for (size_t p = 0; p < ST; ++p) {
        size_t tmp = p;
        for (int i = 6; i >= 0; --i) {
            digits[p][i] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }
        int h = 0;
        for (int i = 0; i < 6; ++i)
            if (digits[p][i] + digits[p][i + 1] == 2)
                ++h;
        horiz[p] = h;
    }

    /* DP arrays for the current and next row.  Initially row 1.       */
    seq_t *dp0[2] = { (seq_t *)calloc(ST, sizeof(seq_t)),
                      (seq_t *)calloc(ST, sizeof(seq_t)) };
    seq_t *dp1[2] = { (seq_t *)calloc(ST, sizeof(seq_t)),
                      (seq_t *)calloc(ST, sizeof(seq_t)) };
    int cur = 0, nxt = 1;

    /* Row 1: only horizontal adjacency contributes.                 */
    for (size_t p = 0; p < ST; ++p) {
        int h = horiz[p];
        if (h == 0)
            dp0[cur][p] = (seq_t)1;
        else if (h == 1)
            dp1[cur][p] = (seq_t)1;
    }

    /* Sum over patterns to obtain a(1).                               */
    seq_t sum = 0;
    for (size_t p = 0; p < ST; ++p)
        sum += dp1[cur][p];
    init[0] = sum;

    /* Rows 2 … 8.  Each iteration extends the DP one more row.        */
    for (size_t row = 2; row <= 8; ++row) {
        /* Swap roles of current and next arrays.                       */
        int tmp = cur; cur = nxt; nxt = tmp;
        /* Zero the destination arrays.                                */
        for (size_t i = 0; i < ST; ++i) {
            dp0[cur][i] = 0;
            dp1[cur][i] = 0;
        }

        /* Transition from all previous patterns to all current ones.  */
        for (size_t prev = 0; prev < ST; ++prev) {
            seq_t ways0 = dp0[nxt][prev];
            seq_t ways1 = dp1[nxt][prev];
            if (ways0 == 0 && ways1 == 0)
                continue;
            for (size_t curp = 0; curp < ST; ++curp) {
                int h = horiz[curp];

                /* anti‑diagonal contribution between prev and cur.    */
                int a = 0;
                for (int j = 1; j <= 6; ++j)
                    if (digits[prev][j - 1] + digits[curp][j] == 2)
                        ++a;

                /* Transitions from states with 0 total pairs so far.  */
                if (ways0) {
                    int nk = h + a;            /* new total pair count */
                    if (nk == 0)
                        dp0[cur][curp] += ways0;
                    else if (nk == 1)
                        dp1[cur][curp] += ways0;
                }

                /* Transitions from states that already contain 1 pair. */
                if (ways1 && (h + a) == 0)
                    dp1[cur][curp] += ways1;
            }
        }

        /* Count the arrays with exactly one pair after this row.     */
        seq_t cur_sum = 0;
        for (size_t p = 0; p < ST; ++p)
            cur_sum += dp1[cur][p];
        init[row - 1] = cur_sum;                /* a(row) */
    }

    free(dp0[0]); free(dp0[1]); free(dp1[0]); free(dp1[1]);
}

/* -----  Memoised recurrence computation  ----- */
static seq_t *cache   = NULL;  /* memoised sequence values */
static size_t  cache_size = 0; /* number of computed terms */
static size_t  cache_cap  = 0; /* capacity of allocated block */

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of range */

    size_t idx = (size_t)n;

    /* Initialisation on first call. */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
        seq_t init[8];
        compute_initial_terms(init);
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_size = 8;
    }

    /* Return memoised value if already computed. */
    if (idx <= cache_size)
        return cache[idx - 1];

    /* Compute missing terms up to idx using the recurrence. */
    ensure_capacity(idx);
    while (cache_size < idx) {
        /* Positives part of the recurrence. */
        seq_t pos = cache[cache_size - 1] * (seq_t)84;
        pos += cache[cache_size - 3] * (seq_t)31428;
        pos += cache[cache_size - 5] * (seq_t)750384;
        pos += cache[cache_size - 7] * (seq_t)1574640;

        /* Negatives part of the recurrence. */
        seq_t neg = cache[cache_size - 2] * (seq_t)2466;
        neg += cache[cache_size - 4] * (seq_t)206469;
        neg += cache[cache_size - 6] * (seq_t)1513404;
        neg += cache[cache_size - 8] * (seq_t)656100;

        /* Subtract the negative part.  The final result is guaranteed
           non‑negative for the sequence, so unsigned subtraction is safe. */
        seq_t val = pos;
        val -= neg;

        cache[cache_size++] = val;
    }

    return cache[idx - 1];
}
