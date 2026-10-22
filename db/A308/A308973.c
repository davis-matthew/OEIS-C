/*
 * A308973  Langton's ant on a truncated square tiling: number of black cells
 *          after n moves of the ant when starting on an octagon and looking
 *          towards an edge where the tile meets another octagon.
 * https://oeis.org/A308973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5,
 *            a(6) = 6, * a(7) = 5, a(8) = 6, a(9) = 7, a(10) = 8, a(11) = 9,
 *            a(12) = 8, a(13) = 7, * a(14) = 8, a(15) = 9, a(16) = 10, a(17)
 *            = 11, a(18) = 10, a(19) = 11, * a(20) = 10, a(21) = 9, a(22) =
 *            10, a(23) = 11. * For n >= 45: * a(n) = 2*a(...
 * Range:     u8 n <= 500, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- compact hash table for tile colors (0=white, 1=black) --- */
typedef struct {
    uint64_t key;   /* packed (x<<32) | y */
    uint8_t  color;
    uint8_t  used;  /* 0 = empty, 1 = occupied */
} Entry;

static Entry *hash_tbl = NULL;
static uint64_t hash_size = 0, hash_mask = 0;

/* Return a packed key from x,y.  Coordinates are kept 32 bits each. */
static inline uint64_t pack_key(int64_t x, int64_t y)
{
    return (((uint64_t)(uint32_t)x) << 32) | (uint32_t)y;
}

static void init_hash(uint64_t size_pow2)
{
    hash_size = size_pow2;
    hash_mask = hash_size - 1;
    hash_tbl = (Entry*)calloc(hash_size, sizeof(Entry));
}

static uint64_t find_index(uint64_t key)
{
    uint64_t idx = key & hash_mask;
    while (hash_tbl[idx].used && hash_tbl[idx].key != key) {
        idx = (idx + 1) & hash_mask;
    }
    return idx;
}

static uint8_t get_color(int64_t x, int64_t y)
{
    uint64_t key = pack_key(x, y);
    uint64_t idx = find_index(key);
    if (!hash_tbl[idx].used) return 0;
    return hash_tbl[idx].color;
}

static void set_color(int64_t x, int64_t y, uint8_t color)
{
    uint64_t key = pack_key(x, y);
    uint64_t idx = find_index(key);
    hash_tbl[idx].used = 1;
    hash_tbl[idx].key  = key;
    hash_tbl[idx].color = color;
}

/* --- directions for movement on truncated square tiling --- */
static const int   dir_dx[8] = { 2, 1, 0, -1, -2, -1,  0,  1 };
static const int   dir_dy[8] = { 0, 1, 2,  1,  0, -1, -2, -1 };

/* --- pre–compute the first 45 terms using a single ant simulation --- */
static seq_t init_seq_vals[45];
static bool init_done = false;

static void init_seq(void)
{
    if (init_done) return;
    init_done = true;

    /* Estimated space needed for the hash table */
    init_hash(1ULL << 16); /* 65536 buckets */

    seq_t count = 0;
    init_seq_vals[0] = 0;           /* a(0) */

    /* Ant starting state: on an octagon at (1,1), facing East (dir=0) */
    int64_t x = 1, y = 1;
    uint8_t dir = 0;

    for (int i = 1; i <= 44; ++i) {
        uint8_t tile_type = (x & 1);          /* 1 ⇒ octagon, 0 ⇒ square */
        uint8_t color     = get_color(x, y);   /* 0=white, 1=black */

        /* Turn according to tile type and current color */
        if (tile_type) {                      /* octagon */
            dir = (color == 0) ? (dir + 7) & 7   /* 45° right */
                                : (dir + 1) & 7;  /* 45° left  */
        } else {                              /* square */
            dir = (color == 0) ? (dir + 6) & 7   /* 90° right */
                                : (dir + 2) & 7;  /* 90° left  */
        }

        /* Flip the tile color and update the black cell counter */
        uint8_t new_color = 1 - color;
        set_color(x, y, new_color);
        if (new_color) count++; else count--;

        /* Move forward one unit */
        x += dir_dx[dir];
        y += dir_dy[dir];

        init_seq_vals[i] = count;  /* store a(i) */
    }
}

/* --- public sequence function --- */
seq_t a(idx_t n)
{
    init_seq();  /* ensure initial values are ready */

    if (n < 45) return init_seq_vals[n];

    /* Buffer holding the last 12 computed terms for the recurrence */
    seq_t buf[12];
    int start = 33;                 /* indices 33..44 are required for a(45) */
    for (int j = 0; j < 12; ++j) buf[j] = init_seq_vals[start + j];

    seq_t val = 0;
    for (idx_t i = 45; i <= n; ++i) {
        val = 2 * buf[11] - buf[10] - buf[9]
             + 2 * buf[8] - buf[7] - buf[6]
             + 2 * buf[5] - buf[4] - buf[3]
             + 2 * buf[2] - buf[1];

        /* Slide window left, insert new value */
        for (int t = 0; t < 11; ++t) buf[t] = buf[t + 1];
        buf[11] = val;
    }
    return val;
}
