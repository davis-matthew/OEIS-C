/*
 * A370893  Rectangular array, read by antidiagonals: row n consists of the
 *          numbers m whose ternary representation has exactly n runs.
 * https://oeis.org/A370893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = read antidiagonal of array where row r lists numbers
 *            whose ternary representation has exactly r runs
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

static const unsigned long long CAP = ULLONG_MAX / 2;
#define MAX_LEN 512
#define MAX_RUN 512

static unsigned long long C[MAX_LEN + 1][MAX_LEN + 1];
static unsigned long long POW2[MAX_RUN + 1];
static unsigned long long DP[MAX_LEN + 1][MAX_RUN + 1][3];
static bool tables_initialized = false;

static void init_tables(void)
{
    if (tables_initialized) return;
    tables_initialized = true;

    /* binomial coefficients with cap */
    C[0][0] = 1;
    for (int n = 1; n <= MAX_LEN; ++n) {
        C[n][0] = 1;
        C[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            unsigned long long sum = C[n-1][k-1] + C[n-1][k];
            C[n][k] = (sum > CAP) ? CAP : sum;
        }
    }

    /* powers of two with cap */
    POW2[0] = 1;
    for (int i = 1; i <= MAX_RUN; ++i) {
        unsigned long long val = POW2[i-1] << 1;
        POW2[i] = (val > CAP) ? CAP : val;
    }

    /* DP for counting completions: DP[len][runs_needed][prev] */
    for (int prev = 0; prev < 3; ++prev) {
        DP[0][0][prev] = 1;
        for (int r = 1; r <= MAX_RUN; ++r) DP[0][r][prev] = 0;
    }
    for (int len = 1; len <= MAX_LEN; ++len) {
        for (int prev = 0; prev < 3; ++prev) {
            for (int r = 0; r <= MAX_RUN; ++r) {
                unsigned long long sum = 0;
                for (int nd = 0; nd < 3; ++nd) {
                    if (nd == prev) {
                        sum += DP[len-1][r][prev];
                    } else {
                        if (r > 0) sum += DP[len-1][r-1][nd];
                    }
                    if (sum > CAP) { sum = CAP; break; }
                }
                DP[len][r][prev] = sum;
            }
        }
    }
}

/* count how many strings of length L have exactly r runs */
static unsigned long long count_strings(int L, int r)
{
    if (L < r) return 0;
    unsigned long long comb = C[L-1][r-1];
    unsigned long long pow = POW2[r];
    if (comb > CAP / pow) return CAP;
    return comb * pow;
}

/* compute the c-th number (1‑based) in row r (also 1‑based) */
static seq_t row_value(unsigned int r, unsigned int c)
{
    init_tables();

    /* find minimal length L */
    int L;
    int cumulative = 0;
    for (L = r; L <= MAX_LEN; ++L) {
        unsigned long long cnt = count_strings(L, r);
        if (cumulative + (int)cnt >= (int)c) break;
        cumulative += (int)cnt;
    }
    int idx = (int)c - cumulative - 1;                /* 0‑based index within this length */

    /* build the digit sequence */
    seq_t value = 0;
    unsigned long long runs_left = r - 1;          /* runs still to create after first digit */
    int rem_len = L - 1;                          /* digits remaining after first digit */

    /* first digit (cannot be 0) */
    int first_digit;
    for (first_digit = 1; first_digit <= 2; ++first_digit) {
        unsigned long long cnt = DP[rem_len][runs_left][first_digit];
        if (idx < (int)cnt) break;
        idx -= (int)cnt;
    }
    value = first_digit;
    int prev = first_digit;

    /* remaining digits */
    for (int pos = 1; pos < L; ++pos) {
        rem_len = L - 1 - pos;
        int chosen = -1;
        for (int d = 0; d <= 2; ++d) {
            unsigned long long cnt;
            if (d == prev) {
                cnt = DP[rem_len][runs_left][prev];
            } else {
                if (runs_left == 0) { cnt = 0; }
                else cnt = DP[rem_len][runs_left-1][d];
            }
            if (idx < (int)cnt) {
                chosen = d;
                if (d != prev) --runs_left;
                prev = d;
                break;
            }
            idx -= (int)cnt;
        }
        value = value * (seq_t)3 + (seq_t)chosen;
    }
    return value;
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;          /* undefined for 0, but safe */

    /* find diagonal index m such that m*(m+1)/2 >= n */
    idx_t m = 1;
    while ((__int128)m * (m + 1) / 2 < n) ++m;

    idx_t prev_diag = m * (m - 1) / 2;             /* terms before current diagonal */
    idx_t pos_in_diag = n - prev_diag;            /* 1‑based position within diagonal */
    unsigned int r = (unsigned int)pos_in_diag;    /* row number */
    unsigned int c = (unsigned int)(m + 1) - r;    /* column number (index within row) */

    return row_value(r, c);
}
