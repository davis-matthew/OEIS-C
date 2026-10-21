/*
 * A253008  Number of nX5 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 3 and every value
 *          increasing by 0 or 1 with every step right, diagonally se or down.
 * https://oeis.org/A253008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 19, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253008
 *  a(n) for n>12 is given by the cubic polynomial
 *        2973696*n³ - 70716096*n² + 570494664*n - 1560617160
 *  For n ≤ 12 a(n) is computed by dynamic programming on the 5‑column
 *  arrays satisfying the monotonicity rules. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----  constants ----------------------------------------------------- */
#define BASE      12          /* one more than the largest value that can
                               appear for n ≤ 12 (maxV = 11) */
#define CODE_SIZE (BASE*BASE*BASE*BASE*BASE)   /* 12⁵ = 248832 */
#define MAX_V     11          /* maximal value that can appear for n ≤ 12 */

/* ----  pre‑computed decoding table ----------------------------------- */
static uint8_t decode_vec[CODE_SIZE][5];
static int  decode_inited = 0;
static void init_decode(void)
{
    for (int code = 0; code < CODE_SIZE; ++code) {
        int t = code;
        for (int k = 4; k >= 0; --k) {
            decode_vec[code][k] = t % BASE;
            t /= BASE;
        }
    }
    decode_inited = 1;
}

/* ----  encode a 5‑tuple of 0..11 into a code -------------------------- */
static inline int encode_vec(const uint8_t v[5])
{
    int code = 0;
    for (int i = 0; i < 5; ++i)
        code = code * BASE + v[i];
    return code;
}

/* ----  recursive generator of possible next row --------------------- */
static void rec_generate(const uint8_t *prev, int pos, uint8_t *curr,
                         seq_t add, seq_t *next)
{
    if (pos == 5) {
        int code = encode_vec(curr);
        next[code] += add;
        return;
    }
    int low  = prev[pos];
    int high = prev[pos] + 1;
    if (high > MAX_V) high = MAX_V;

    if (pos > 0) {
        if (curr[pos-1] > low) low  = curr[pos-1];
        if (prev[pos-1] > low) low  = prev[pos-1];

        int h = curr[pos-1] + 1;
        if (h < high) high = h;
        h = prev[pos-1] + 1;
        if (h < high) high = h;
    }

    if (low > high) return;     /* no feasible value */

    for (int val = low; val <= high; ++val) {
        curr[pos] = (uint8_t)val;
        rec_generate(prev, pos+1, curr, add, next);
    }
}

static void generate_next_row(const uint8_t *prev, seq_t add, seq_t *next)
{
    uint8_t curr[5] = {0};
    rec_generate(prev, 0, curr, add, next);
}

/* ----  main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* target value at the lower right corner */
    seq_t target = (n <= 4) ? (seq_t)1 : (seq_t)(n - 4);

    /* --------------------------------------------------------------- */
    /*  for n ≤ 12 use DP                                         */
    /* --------------------------------------------------------------- */
    if (n <= 12) {
        if (!decode_inited) init_decode();

        /* allocate arrays for current and next row counts */
        seq_t *cur  = (seq_t *)calloc(CODE_SIZE, sizeof(seq_t));
        seq_t *next = (seq_t *)calloc(CODE_SIZE, sizeof(seq_t));

        /* ----------  initialise first row  ------------------------ */
        for (int code = 0; code < CODE_SIZE; ++code) {
            if (decode_vec[code][0] != 0) continue;          /* a[1,1] = 0   */
            int ok = 1;
            for (int j = 1; j < 5; ++j)
                if (decode_vec[code][j] < decode_vec[code][j-1] ||
                    decode_vec[code][j] > decode_vec[code][j-1]+1)
                    { ok = 0; break; }
            if (ok) cur[code] = 1;
        }

        /* ----------  process remaining rows  --------------------- */
        for (idx_t row = 2; row <= n; ++row) {
            memset(next, 0, CODE_SIZE * sizeof(seq_t));
            for (int code = 0; code < CODE_SIZE; ++code) {
                seq_t cnt = cur[code];
                if (!cnt) continue;
                const uint8_t *prev = decode_vec[code];
                generate_next_row(prev, cnt, next);
            }
            seq_t *tmp = cur; cur = next; next = tmp;
        }

        /* ----------  sum arrays that finish with the target value  */
        seq_t result = 0;
        for (int code = 0; code < CODE_SIZE; ++code) {
            seq_t cnt = cur[code];
            if (!cnt) continue;
            if (decode_vec[code][4] == target) result += cnt;
        }

        free(cur); free(next);
        return result;
    }

    /* --------------------------------------------------------------- */
    /*  for n > 12 use cubic polynomial                               */
    /* --------------------------------------------------------------- */
    int64_t tn = (int64_t)n;
    int64_t r = 2973696LL*tn*tn*tn
                - 70716096LL*tn*tn
                + 570494664LL*tn
                - 1560617160LL;
    return (seq_t)r;
}
