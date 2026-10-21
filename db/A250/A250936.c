/*
 * A250936  Number of (n+1)X(2+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250936
 *  Number of (n+1)×3 arrays over {0,1,2}
 *  satisfying:
 *      1. x(i,j)+x(i,j-1) is non‑decreasing in the row index i
 *      2. |x(i,j)-x(i-1,j)| is non‑decreasing in the column index j
 *  The code counts such arrays by dynamic programming over the rows.
 *  No lookup table of initial terms is used.
 */

#include <stddef.h>
#include <stdint.h>

#define ROWS 27

static uint8_t _row[ROWS][3];          /* all possible row vectors (0..2)^3 */
static int    _s1[ROWS], _s2[ROWS];     /* sums x0+x1 and x1+x2 for each row */
static int    _allowed[ROWS][ROWS];    /* transition table: 1 = u -> v allowed */

/* initialise static tables once */
static void _init_tables(void)
{
    static int _inited = 0;
    if (_inited) return;
    _inited = 1;

    for (size_t k = 0; k < ROWS; ++k) {
        int r = (int)k;
        _row[k][0] = r % 3;   r /= 3;
        _row[k][1] = r % 3;   r /= 3;
        _row[k][2] = r % 3;
        _s1[k] = _row[k][0] + _row[k][1];
        _s2[k] = _row[k][1] + _row[k][2];
    }

    for (size_t u = 0; u < ROWS; ++u) {
        for (size_t v = 0; v < ROWS; ++v) {
            if (_s1[u] <= _s1[v] && _s2[u] <= _s2[v]) {
                int d0 = (_row[v][0] > _row[u][0]) ? (_row[v][0] - _row[u][0]) : (_row[u][0] - _row[v][0]);
                int d1 = (_row[v][1] > _row[u][1]) ? (_row[v][1] - _row[u][1]) : (_row[u][1] - _row[v][1]);
                int d2 = (_row[v][2] > _row[u][2]) ? (_row[v][2] - _row[u][2]) : (_row[u][2] - _row[v][2]);
                _allowed[u][v] = (d0 <= d1 && d1 <= d2) ? 1 : 0;
            } else {
                _allowed[u][v] = 0;
            }
        }
    }
}

/* ------------------------------------------------------------ */
/* Return the n-th term of the sequence (offset 1).  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* out of range, but keep defined */

    _init_tables();

    size_t N = (size_t)n;
    seq_t dp_cur[ROWS], dp_next[ROWS];

    /* row 0 : any vector is allowed */
    for (size_t v = 0; v < ROWS; ++v)
        dp_cur[v] = (seq_t)1;

    /* iterate through rows 1 .. N */
    for (size_t step = 1; step <= N; ++step) {
        /* reset dp_next */
        for (size_t v = 0; v < ROWS; ++v)
            dp_next[v] = (seq_t)0;

        /* transitions from previous row (dp_cur) to current row (dp_next) */
        for (size_t u = 0; u < ROWS; ++u) {
            seq_t val = dp_cur[u];
            if (val == (seq_t)0) continue;            /* skip zero contributions */
            for (size_t v = 0; v < ROWS; ++v) {
                if (_allowed[u][v]) dp_next[v] += val;
            }
        }

        /* if we reached the last row, compute total */
        if (step == N) {
            seq_t total = (seq_t)0;
            for (size_t v = 0; v < ROWS; ++v)
                total += dp_next[v];
            return total;
        }

        /* prepare for next step */
        for (size_t v = 0; v < ROWS; ++v)
            dp_cur[v] = dp_next[v];
    }

    /* should not reach here */
    return (seq_t)0;
}
