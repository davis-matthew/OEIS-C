/*
 * A297098  Number of nX4 0..1 arrays with no 1 adjacent to 4 king-move
 *          neighboring 1s.
 * https://oeis.org/A297098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with no '1' having exactly
 *            four king‑move neighbors that are '1'. The function uses a
 *            dynamic‑programming sweep over the rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Pre‑computed table valid[prev][curr][next] is true iff in the
   triple of rows (prev, curr, next) every '1' in 'curr' has a
   number of king‑move neighbouring '1's different from four. */
static bool valid[16][16][16];
static bool valid_initialized = false;

static void init_valid(void)
{
    if (valid_initialized)
        return;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            for (int next = 0; next < 16; ++next) {
                bool ok = true;
                for (int col = 0; col < 4; ++col) {
                    if ((curr >> col) & 1) {
                        int cnt = 0;
                        /* same row */
                        if (col > 0 && ((curr >> (col - 1)) & 1)) ++cnt;
                        if (col < 3 && ((curr >> (col + 1)) & 1)) ++cnt;
                        /* upper row */
                        if ((prev >> col) & 1) ++cnt;
                        if (col > 0 && ((prev >> (col - 1)) & 1)) ++cnt;
                        if (col < 3 && ((prev >> (col + 1)) & 1)) ++cnt;
                        /* lower row */
                        if ((next >> col) & 1) ++cnt;
                        if (col > 0 && ((next >> (col - 1)) & 1)) ++cnt;
                        if (col < 3 && ((next >> (col + 1)) & 1)) ++cnt;
                        if (cnt == 4) { ok = false; break; }
                    }
                }
                valid[prev][curr][next] = ok;
            }
        }
    }
    valid_initialized = true;
}

/* -------------- the sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    init_valid();                     /* ensure the lookup table is ready */

    if (n == 0)                      /* undefined by the OEIS offset */
        return (seq_t)0;

    /* --------------------------------------------------- n == 1 --------------- */
    if (n == 1) {
        seq_t sum = 0;
        for (int cur = 0; cur < 16; ++cur)
            if (valid[0][cur][0])
                sum += 1;
        return sum;
    }

    /* --------------------------------------------------- n == 2 --------------- */
    if (n == 2) {
        seq_t sum = 0;
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2)
                if (valid[0][r1][r2] && valid[r1][r2][0])
                    sum += 1;
        return sum;
    }

    /* --------------------------------------------------- n >= 3 --------------- */
    const size_t STATE_COUNT = 256;          /* 2^8 states (two rows) */
    seq_t dp_cur[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];
    seq_t zero = 0;
    seq_t result = 0;

    /* zero initialise dp_cur and dp_next */
    for (size_t i = 0; i < STATE_COUNT; ++i) {
        dp_cur[i] = zero;
        dp_next[i] = zero;
    }

    /* Initialisation: states after the first two rows. */
    for (int r1 = 0; r1 < 16; ++r1)
        for (int r2 = 0; r2 < 16; ++r2)
            if (valid[0][r1][r2]) {
                size_t idx = ((size_t)r1 << 4) | (size_t)r2;
                dp_cur[idx] = 1;
            }

    /* Add rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        for (size_t i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = zero;
        for (size_t state = 0; state < STATE_COUNT; ++state) {
            seq_t cnt = dp_cur[state];
            if (cnt == zero)
                continue;
            int prev = (int)(state >> 4);
            int curr = (int)(state & 0xF);
            for (int next_row = 0; next_row < 16; ++next_row) {
                if (!valid[prev][curr][next_row])
                    continue;
                size_t new_state = ((size_t)curr << 4) | (size_t)next_row;
                dp_next[new_state] += cnt;
            }
        }
        for (size_t i = 0; i < STATE_COUNT; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Final check for the last row (no neighbours below). */
    for (size_t state = 0; state < STATE_COUNT; ++state) {
        seq_t cnt = dp_cur[state];
        if (cnt == zero)
            continue;
        int prev = (int)(state >> 4);
        int curr = (int)(state & 0xF);
        if (valid[prev][curr][0])
            result += cnt;
    }
    return result;
}
