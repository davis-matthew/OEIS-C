/*
 * A282315  Number of nX7 0..1 arrays with no 1 equal to more than four of its
 *          king-move neighbors.
 * https://oeis.org/A282315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of n×7 binary arrays with no 1 having >4 king‑move neighbors */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Width of each row */
#define WIDTH 7
/* Number of possible rows (2^7 = 128) */
#define STATES (1u << WIDTH)

/* ------------------------------------------------------------------ */
/*  Precomputation of valid transitions between 3 consecutive rows  */
/* ------------------------------------------------------------------ */
static bool tables_initialized = false;

/* How many valid next rows are possible for [prev][cur] */
static unsigned char next_count[STATES][STATES];
/* List of those next rows */
static unsigned char next_list[STATES][STATES][STATES];
/* Whether [prev][cur] can be the last two rows (cur is last row, nxt == 0) */
static bool last_valid[STATES][STATES];

/* Test whether a 1 in 'cur' has >4 king‑move neighbours,
   where 'prev' and 'next' are the rows above and below. */
static bool is_valid_row(uint8_t prev, uint8_t cur, uint8_t next)
{
    for (uint8_t col = 0; col < WIDTH; ++col) {
        if (!(cur & (1u << col))) continue;          /* cell is 0 */
        unsigned cnt = 0;

        /* same row */
        if (col > 0        && (cur & (1u << (col - 1)))) cnt++;
        if (col + 1 < WIDTH && (cur & (1u << (col + 1)))) cnt++;

        /* above row */
        if (col > 0        && (prev & (1u << (col - 1)))) cnt++;
        if (prev & (1u << col))                       cnt++;
        if (col + 1 < WIDTH && (prev & (1u << (col + 1)))) cnt++;

        /* below row */
        if (col > 0        && (next & (1u << (col - 1)))) cnt++;
        if (next & (1u << col))                       cnt++;
        if (col + 1 < WIDTH && (next & (1u << (col + 1)))) cnt++;

        if (cnt > 4) return false;
    }
    return true;
}

static void init_tables(void)
{
    if (tables_initialized) return;

    for (uint8_t prev = 0; prev < STATES; ++prev) {
        for (uint8_t cur = 0; cur < STATES; ++cur) {
            unsigned cnt = 0;
            for (uint8_t nxt = 0; nxt < STATES; ++nxt) {
                if (is_valid_row(prev, cur, nxt)) {
                    next_list[prev][cur][cnt++] = nxt;
                }
            }
            next_count[prev][cur] = (unsigned char)cnt;
            last_valid[prev][cur] = is_valid_row(prev, cur, 0);
        }
    }
    tables_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Dynamic programming over rows (two 128×128 tables)                */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t cache_n = 0;            /* number of terms stored in cache   */
static size_t cache_cap = 0;         /* allocated cache size              */

static seq_t (*dp_last)[STATES];
static seq_t (*dp_next)[STATES];
static seq_t dp_a[STATES][STATES];
static seq_t dp_b[STATES][STATES];
static bool dp_ready = false;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; harmless return */

    init_tables();

    /* initialise DP arrays on first call */
    if (!dp_ready) {
        dp_last = dp_a;
        dp_next = dp_b;
        /* zero all entries */
        for (uint8_t i = 0; i < STATES; ++i)
            for (uint8_t j = 0; j < STATES; ++j)
                dp_last[i][j] = 0;
        /* first row : 'prev' is all zeros */
        for (uint8_t mask = 0; mask < STATES; ++mask)
            dp_last[0][mask] = 1;
        dp_ready = true;
    }

    /* Allocate cache if necessary */
    if (cache_cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed */
        cache_cap = 1;
    }

    /* Compute and store a(1) if not yet cached */
    if (cache_n == 0) {
        seq_t total1 = 0;
        for (uint8_t cur = 0; cur < STATES; ++cur) {
            if (last_valid[0][cur]) {
                total1 += dp_last[0][cur];   /* dp_last[0][cur] is 1 for each cur */
            }
        }
        cache[0] = total1;
        cache_n = 1;
    }

    /* extend cache up to requested n */
    while (cache_n < n) {
        /* clear dp_next */
        for (uint8_t i = 0; i < STATES; ++i)
            for (uint8_t j = 0; j < STATES; ++j)
                dp_next[i][j] = 0;

        /* transition from dp_last to dp_next */
        for (uint8_t prev = 0; prev < STATES; ++prev) {
            for (uint8_t cur = 0; cur < STATES; ++cur) {
                seq_t val = dp_last[prev][cur];
                if (val == 0) continue;
                unsigned char cnt = next_count[prev][cur];
                for (unsigned char k = 0; k < cnt; ++k) {
                    uint8_t nxt = next_list[prev][cur][k];
                    dp_next[cur][nxt] += val;
                }
            }
        }

        /* total for the new height (cache_n + 1) */
        seq_t total = 0;
        for (uint8_t prev = 0; prev < STATES; ++prev) {
            for (uint8_t cur = 0; cur < STATES; ++cur) {
                if (last_valid[prev][cur])
                    total += dp_next[prev][cur];
            }
        }

        /* make room in cache if needed */
        if (cache_n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_n] = total;
        ++cache_n;

        /* swap dp_last and dp_next for the next iteration */
        seq_t (*tmp_tbl)[STATES] = dp_last;
        dp_last = dp_next;
        dp_next = tmp_tbl;
    }

    return cache[n - 1];
}
