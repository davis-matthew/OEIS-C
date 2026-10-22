/*
 * A379821  Array read by ascending antidiagonals: A(n, k) = (-1)^(n + k) *
 *          Sum_{j=0..k} (j!)^2 * Stirling1(n, j) * Stirling1(k, j).
 * https://oeis.org/A379821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Array read by ascending antidiagonals: 
 * A(n,k) = ∑_{j=0..k} (j!)^2 * Stirling1_unsigned(n,j) * Stirling1_unsigned(k,j)
 * (the sign factor from the OEIS definition cancels with the signs of
 * the signed Stirling numbers of the first kind).
 * The sequence a(n) is obtained by reading the entries A(i,j) in
 * antidiagonal order with indexing starting at 0.
 */

#define MAX_S 200      /* sufficient for n ≤ 10000 */

static seq_t fact[MAX_S + 1];
static seq_t stir[MAX_S + 1][MAX_S + 1];
static int initialized = 0;

seq_t a(idx_t n)
{
    /* initialise factorials and unsigned Stirling numbers of the first kind */
    if (!initialized) {
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= MAX_S; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;

        for (idx_t i = 0; i <= MAX_S; ++i)
            for (idx_t j = 0; j <= MAX_S; ++j)
                stir[i][j] = (seq_t)0;

        stir[0][0] = (seq_t)1;
        for (idx_t i = 1; i <= MAX_S; ++i)
            for (idx_t j = 1; j <= i; ++j) {
                seq_t term = stir[i - 1][j];
                term *= (seq_t)(i - 1);          /* (i-1) * stir[i-1][j] */
                stir[i][j] = stir[i - 1][j - 1] + term;
            }

        initialized = 1;
    }

    /* locate (i,j) on antidiagonal s for index n */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    idx_t t = n - s * (s + 1) / 2;   /* t ranges 0..s */
    idx_t i = t;                     /* row index */
    idx_t j = s - t;                 /* column index */

    /* compute A(i,j) */
    seq_t result = (seq_t)0;
    for (idx_t u = 0; u <= j; ++u) {
        seq_t term = fact[u];
        term *= fact[u];                 /* (u!)^2 */
        term *= stir[i][u];
        term *= stir[j][u];
        result += term;
    }

    return result;
}
