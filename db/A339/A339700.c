/*
 * A339700  a(n) is the n-th nonnegative number to light exactly n segments
 *          when displayed on a calculator.
 * https://oeis.org/A339700
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th nonnegative integer that lights exactly n *
 *            seven‑segment display segments (calculator digits). The function
 *            * returns a(n) for n >= 5. For n < 5 the value is undefined; we
 *            * simply return 0 to keep the function callable
 * Range:     u8 n <= 15, u16 n <= 32, u32 n <= 66, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint8_t seg[10] = {6,2,5,5,4,5,6,3,7,6};

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;                     // undefined for offset < 5

    /* The required segment sum equals the index itself. */
    int sum = (int)n;                 // sum <= 10000 in typical tests
    if (sum < 0)                      // safety clamp for huge n
        return 0;

    /* We will count numbers of each length until we reach the desired
     * n‑th element.  DP rows are built incrementally and stored,
     * but we stop once the cumulative count exceeds the target. */
    const uint32_t INF = (uint32_t)sum + 1;  // saturation value
    /* dpRows[len][s] = number of ways to form len digits (each 0‑9)
     * with total segment sum s.  Length 0 has one way for sum 0. */
    size_t cols = (size_t)sum + 1;
    /* Vector of row arrays; dpRows[0] is the row for length 0. */
    struct Row { uint16_t *data; };
    struct Row *dpRows = NULL;
    size_t dpRowCount = 0;
    /* Allocate first row */
    dpRows = (struct Row *)malloc(sizeof(struct Row) * ((size_t)(sum / 2) + 2));
    if (!dpRows) return 0;            /* allocation failed */
    dpRows[0].data = (uint16_t *)calloc(cols, sizeof(uint16_t));
    if (!dpRows[0].data) return 0;
    dpRows[0].data[0] = 1;
    dpRowCount = 1;

    uint32_t cumulative = 0;          // total numbers seen so far
    uint32_t countLen = 0;            // numbers of current length
    size_t targetLen = 0;             // length of answer
    uint32_t rankWithinLen = 0;       // 1‑based rank within this length

    for (size_t len = 1; ; ++len) {
        /* Build DP row for length len */
        uint16_t *row = (uint16_t *)calloc(cols, sizeof(uint16_t));
        if (!row) return 0;
        for (int s = 0; s <= sum; ++s) {
            uint32_t val = 0;
            for (int d = 0; d <= 9; ++d) {
                if (s >= seg[d]) {
                    val += dpRows[len-1].data[s - seg[d]];
                    if (val >= INF) { val = INF; break; }
                }
            }
            row[s] = (uint16_t)val;
        }
        /* Store the row */
        dpRows[dpRowCount].data = row;
        ++dpRowCount;

        /* Count numbers with exactly 'len' digits that sum to 'sum' */
        countLen = 0;
        if (len == 1) {
            for (int d = 0; d <= 9; ++d) {
                if (seg[d] == sum) {
                    ++countLen;
                    if (countLen >= INF) { countLen = INF; break; }
                }
            }
        } else {
            for (int d = 1; d <= 9; ++d) {     /* leading digit > 0 */
                if (seg[d] <= sum) {
                    uint32_t cnt = dpRows[len-1].data[sum - seg[d]];
                    countLen += cnt;
                    if (countLen >= INF) { countLen = INF; break; }
                }
            }
        }

        cumulative += countLen;
        if (cumulative >= (uint32_t)n) {
            targetLen = len;
            rankWithinLen = (uint32_t)n - (cumulative - countLen);
            break;
        }

        /* Stop if we exhausted maximum possible length */
        if (len > (size_t)(sum / 2) + 1)
            break;
    }

    /* Construct the desired number digit by digit. */
    seq_t value = 0;
    size_t remLen = targetLen;
    int remSum = sum;
    while (remLen > 0) {
        int startDigit = 0;
        if (remLen == targetLen && targetLen > 1)
            startDigit = 1;              /* leading zeros not allowed */
        for (int d = startDigit; d <= 9; ++d) {
            int s = seg[d];
            if (s > remSum) continue;

            uint32_t cnt = 0;
            size_t nextLen = remLen - 1;
            int nextSum = remSum - s;
            if (nextLen == 0) {
                cnt = (nextSum == 0) ? 1 : 0;
            } else {
                cnt = dpRows[nextLen].data[nextSum];
            }

            if (cnt >= rankWithinLen) {
                value = value * 10 + (seq_t)d;
                remLen = nextLen;
                remSum = nextSum;
                break;
            } else {
                rankWithinLen -= cnt;
            }
        }
    }

    /* Clean up */
    for (size_t i = 0; i < dpRowCount; ++i)
        free(dpRows[i].data);
    free(dpRows);

    return value;
}
