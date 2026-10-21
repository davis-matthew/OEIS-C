/*
 * A281343  Number of nX7 0..1 arrays with no element equal to more than three
 *          of its king-move neighbors and with new values introduced in order
 *          0 sequentially upwards.
 * https://oeis.org/A281343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays with first cell 0 and no
 *            element equal to more than three of its king‑move neighbors.
 *            (See OEIS A281343.)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 33, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* -------------------- neighbor test helpers -------------------- */

static inline bool valid_mid(uint8_t upper, uint8_t mid, uint8_t lower)
{
    for (int col = 0; col < 7; ++col) {
        int val = (mid >> col) & 1;
        int equal = 0;

        /* upper neighbors */
        if (col > 0 && ((upper >> (col - 1)) & 1) == val) ++equal;
        if (((upper >> col) & 1) == val)               ++equal;
        if (col < 6 && ((upper >> (col + 1)) & 1) == val) ++equal;

        /* current row neighbors */
        if (col > 0 && ((mid >> (col - 1)) & 1) == val) ++equal;
        if (col < 6 && ((mid >> (col + 1)) & 1) == val) ++equal;

        /* lower neighbors */
        if (col > 0 && ((lower >> (col - 1)) & 1) == val) ++equal;
        if (((lower >> col) & 1) == val)               ++equal;
        if (col < 6 && ((lower >> (col + 1)) & 1) == val) ++equal;

        if (equal > 3) return false;
    }
    return true;
}

static inline bool valid_row(uint8_t upper, uint8_t mid, uint8_t lower,
                             bool has_up, bool has_down)
{
    for (int col = 0; col < 7; ++col) {
        int val = (mid >> col) & 1;
        int equal = 0;

        if (has_up) {
            if (col > 0 && ((upper >> (col - 1)) & 1) == val) ++equal;
            if (((upper >> col) & 1) == val)                      ++equal;
            if (col < 6 && ((upper >> (col + 1)) & 1) == val) ++equal;
        }

        if (col > 0 && ((mid >> (col - 1)) & 1) == val) ++equal;
        if (col < 6 && ((mid >> (col + 1)) & 1) == val) ++equal;

        if (has_down) {
            if (col > 0 && ((lower >> (col - 1)) & 1) == val) ++equal;
            if (((lower >> col) & 1) == val)                      ++equal;
            if (col < 6 && ((lower >> (col + 1)) & 1) == val) ++equal;
        }

        if (equal > 3) return false;
    }
    return true;
}

/* ---------------------- pre‑computed tables -------------------- */

static bool tables_ready = false;
static unsigned char next_count[128][128];
static unsigned char next_rows[128][128][128];
static unsigned char first_ok[128][128];

static void build_tables(void)
{
    /* transition from (upper,mid) to next row 'lower' */
    for (int u = 0; u < 128; ++u) {
        for (int m = 0; m < 128; ++m) {
            unsigned char cnt = 0;
            for (int l = 0; l < 128; ++l) {
                if (valid_mid((uint8_t)u, (uint8_t)m, (uint8_t)l))
                    next_rows[u][m][cnt++] = (unsigned char)l;
            }
            next_count[u][m] = cnt;
        }
    }

    /* first row must have bit0==0; test pairs (f,s) */
    for (int f = 0; f < 128; ++f) {
        if (f & 1) continue;      /* first cell 1 → skip */
        for (int s = 0; s < 128; ++s) {
            if (valid_row(0, (uint8_t)f, (uint8_t)s, false, true))
                first_ok[f][s] = 1;
        }
    }

    tables_ready = true;
}

/* --------------------------- sequence -------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset < 1 */

    if (n == 1) {
        /* 2^6 possibilities for the remaining 6 cells */
        seq_t r = 1;
        for (int i = 0; i < 6; ++i) r *= (seq_t)2;
        return r;
    }

    if (!tables_ready) build_tables();

    /* DP over pairs of rows.  State (prev,curr).  dp holds counts for the
       current step; ndp is used for the next step. */
    static seq_t dp[128][128];
    static seq_t ndp[128][128];
    for (int i = 0; i < 128; ++i)
        for (int j = 0; j < 128; ++j)
            dp[i][j] = ndp[i][j] = 0;

    /* active list of non‑zero states. */
    static unsigned short active[16384];
    static unsigned short new_active[16384];
    int active_cnt = 0, new_active_cnt = 0;

    /* initialise with all valid first pairs */
    for (int f = 0; f < 128; ++f) {
        if (f & 1) continue;
        for (int s = 0; s < 128; ++s) {
            if (!first_ok[f][s]) continue;
            dp[f][s] = (seq_t)1;
            active[active_cnt++] = (unsigned short)((f << 7) | s);
        }
    }

    /* transitions for the middle rows */
    if (n >= 3) {
        int steps = (int)(n - 2);
        for (int step = 0; step < steps; ++step) {
            new_active_cnt = 0;
            for (int idx = 0; idx < active_cnt; ++idx) {
                unsigned short pair = active[idx];
                unsigned short prev = pair >> 7;
                unsigned short curr = pair & 127;
                seq_t val = dp[prev][curr];
                if (val == 0) continue;

                unsigned char cnt = next_count[prev][curr];
                unsigned char *list = next_rows[prev][curr];
                for (int k = 0; k < cnt; ++k) {
                    unsigned short nxt = list[k];
                    seq_t *dest = &ndp[curr][nxt];
                    bool was_zero = (*dest == 0);
                    *dest += val;
                    if (was_zero)
                        new_active[new_active_cnt++] = (unsigned short)((curr << 7) | nxt);
                }
            }

            /* swap dp and ndp, clear ndp */
            for (int i = 0; i < 128; ++i)
                for (int j = 0; j < 128; ++j) {
                    dp[i][j] = ndp[i][j];
                    ndp[i][j] = 0;
                }

            active_cnt = new_active_cnt;
            for (int i = 0; i < active_cnt; ++i) active[i] = new_active[i];
        }
    }

    /* sum over final rows */
    seq_t ans = 0;
    for (int idx = 0; idx < active_cnt; ++idx) {
        unsigned short pair = active[idx];
        unsigned short prev = pair >> 7;
        unsigned short curr = pair & 127;
        if (valid_row((uint8_t)prev, (uint8_t)curr, 0, true, false))
            ans += dp[prev][curr];
    }

    return ans;
}
