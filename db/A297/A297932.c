/*
 * A297932  Rectangular array, by antidiagonals: row n gives the numbers whose
 *          base-2 digits d(m), d(m-1),...,d(0) having n as maximal run-length
 *          of 0's.
 * https://oeis.org/A297932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, tabl
 * Formula:   a(n) = numbers enumerated by antidiagonals of the array where
 *            the row index indicates the maximal run-length of zeros in the
 *            binary representation of the numbers. The sequence a(n) is
 *            obtained by reading the array along anti-diagonals: a(1)=1,
 *            a(2)=3, a(3)=2, a(4)=7,
 * Range:     u8 n <= 36, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Limit of DP dimensions (enough for n up to about 10000) */
#define MAX_LEN   400
#define MAX_RUN   300

/* Forward declarations */
static seq_t get_row_element(idx_t row, idx_t k);
static seq_t build_kth(idx_t L, idx_t maxRun, idx_t offset);
static unsigned int count_strings_saturate(idx_t L, idx_t maxRun, unsigned int limit);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;
    idx_t prev = ((d - 1) * d) / 2;
    idx_t k = n - prev;            /* 1-based position inside diagonal d */
    idx_t row = k;                 /* row index of element */
    idx_t col = d + 1 - k;         /* column index within that row */

    return get_row_element(row, col);
}

/* -------------------- Row element retrieval -------------------- */

/* Return the k-th (1-based) smallest integer whose maximal zero-run
   length is (row-1).  For row==1 the numbers are 1,3,7,... */
static seq_t get_row_element(idx_t row, idx_t k)
{
    if (row == 1) {
        /* All-ones numbers: 2^k - 1 */
        return ((seq_t)1 << k) - ((seq_t)1);
    }

    idx_t maxRun = row - 1;              /* maximal zero-run allowed */
    /* We need k-th element; k <= 200 in the use cases */
    unsigned int limit = (unsigned int)k;  /* saturate counts at this value */

    /* Find minimal length L such that cumulative counts reach k */
    idx_t L = row;                       /* minimal possible length */
    idx_t cumulative = 0;
    while (1) {
        unsigned int cnt = count_strings_saturate(L, maxRun, limit);
        if (cumulative + cnt >= k)
            break;
        cumulative += cnt;
        ++L;
    }
    idx_t offset = k - cumulative;      /* 1-based within strings of length L */

    return build_kth(L, maxRun, offset);
}

/* -------------------- Counting strings with DP -------------------- */

/* Return the number of binary strings of length L that:
   - start with 1,
   - contain no run of zeros longer than maxRun,
   - contain at least one run of zeros exactly maxRun.
   Count values are saturated at `limit` (used to stop at k). */
static unsigned int count_strings_saturate(idx_t L, idx_t maxRun, unsigned int limit)
{
    static unsigned int dp[MAX_LEN + 1][MAX_RUN + 1][2];
    /* Base case: position L */
    for (int run = 0; run <= maxRun; ++run) {
        dp[L][run][0] = 0;          /* flag 0 -> invalid at end */
        dp[L][run][1] = 1;          /* flag 1 -> valid at end */
    }

    for (int pos = (int)L - 1; pos >= 1; --pos) {
        for (int run = 0; run <= maxRun; ++run) {
            for (int flag = 0; flag < 2; ++flag) {
                unsigned long long sum = 0;

                /* Place a zero if allowed */
                if (run + 1 <= maxRun) {
                    int newRun = run + 1;
                    int newFlag = flag || (newRun == maxRun);
                    sum += dp[pos + 1][newRun][newFlag];
                }

                /* Place a one */
                sum += dp[pos + 1][0][flag];

                if (sum > (unsigned long long)limit)
                    sum = (unsigned long long)limit + 1;
                dp[pos][run][flag] = (unsigned int)sum;
            }
        }
    }

    /* Count of strings starting after first 1 placed (pos = 1) */
    return dp[1][0][0];
}

/* -------------------- Build k-th string -------------------- */

static seq_t build_kth(idx_t L, idx_t maxRun, idx_t offset)
{
    static unsigned int dp[MAX_LEN + 1][MAX_RUN + 1][2];
    /* Base case */
    for (int run = 0; run <= maxRun; ++run) {
        dp[L][run][0] = 0;
        dp[L][run][1] = 1;
    }
    for (int pos = (int)L - 1; pos >= 1; --pos) {
        for (int run = 0; run <= maxRun; ++run) {
            for (int flag = 0; flag < 2; ++flag) {
                unsigned long long sum = 0;
                if (run + 1 <= maxRun) {
                    int newRun = run + 1;
                    int newFlag = flag || (newRun == maxRun);
                    sum += dp[pos + 1][newRun][newFlag];
                }
                sum += dp[pos + 1][0][flag];
                dp[pos][run][flag] = (unsigned int)sum;
            }
        }
    }

    /* Build the value bit by bit */
    seq_t value = (seq_t)1;           /* first bit is always 1 */
    idx_t pos = 1;                    /* bits placed so far */
    int run = 0;
    int flag = 0;
    int off = (int)offset;            /* offset < 200 */

    for (; pos < L; ++pos) {
        unsigned int cnt0 = 0;
        if (! (run + 1 > maxRun)) {
            int newRun = run + 1;
            int newFlag = flag || (newRun == maxRun);
            cnt0 = dp[pos + 1][newRun][newFlag];
        }

        if (cnt0 >= off) {
            /* Choose zero */
            value = (value << 1);
            run = run + 1;
            flag = flag || (run == maxRun);
        } else {
            /* Choose one */
            off -= cnt0;
            value = (value << 1) | ((seq_t)1);
            run = 0;                 /* reset run of zeros */
            /* flag unchanged */
        }
    }
    return value;
}
