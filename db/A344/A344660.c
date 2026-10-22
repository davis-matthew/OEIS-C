/*
 * A344660  Lexicographically earliest sequence of distinct nonnegative terms
 *          on a square spiral such that each term, when written with one
 *          digit per square, forms no prime value in the eight sums when each
 *          digit is added to each of its eight nearest neighbors.
 * https://oeis.org/A344660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini
 * Formula:   a(n) computed by brute‑force construction of the
 *            lexicographically earliest sequence of distinct non‑negative
 *            integers placed in a square spiral. Each digit placed must
 *            satisfy that the sum of this digit with each of its eight
 *            neighbours is never prime
 * Range:     u8 n <= 37, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

/* Grid large enough for all digits that may be needed */
#define GRID_SIZE 2049
#define OFFSET   (GRID_SIZE/2)

/* Prime lookup for sums 0 … 18 (digit + neighbour)            */
/* 0,1 not prime; 2,3,5,7,11,13,17 are prime; the rest are not */
static const bool isPrime[19] = {
    /*  0 */ false,
    /*  1 */ false,
    /*  2 */ true,  /*  3 */ true,  /*  4 */ false,
    /*  5 */ true,  /*  6 */ false, /*  7 */ true,
    /*  8 */ false, /*  9 */ false, /* 10 */ false,
    /* 11 */ true,  /* 12 */ false, /* 13 */ true,
    /* 14 */ false, /* 15 */ false, /* 16 */ false,
    /* 17 */ true,  /* 18 */ false
};

/* Spiral direction vectors: east, north, west, south          */
static const int dir_dx[4] = { 1, 0, -1, 0 };
static const int dir_dy[4] = { 0, 1, 0, -1 };

seq_t a(idx_t n)
{
    /* --- Static cache for the sequence --- */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;

    /* --- Static grid holding placed digits (255 = empty) --- */
    static unsigned char grid[GRID_SIZE][GRID_SIZE];
    static bool grid_init = false;

    /* --- State of the spiral traversal --- */
    static int cur_x = 0, cur_y = 0;          /* offset from centre */
    static int dir  = 0, step = 1, rem  = 1;  /* direction control   */
    static int dir_changes = 0;                /* for step increment  */

    /* --- Initialise on first call --- */
    if (!grid_init) {
        for (int y = 0; y < GRID_SIZE; ++y)
            for (int x = 0; x < GRID_SIZE; ++x)
                grid[y][x] = 255;                /* empty cell   */
        grid[OFFSET][OFFSET] = 0;              /* first term   */
        seq_cap = 1;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        seq[0] = (seq_t)0;
        seq_len = 1;
        grid_init = true;
    }

    /* --- Build sequence until we have at least n terms --- */
    while (seq_len < n) {
        /* Look for the smallest unused integer that fits */
        for (idx_t cand_i = 0;; ++cand_i) {
            seq_t cand = (seq_t)cand_i;

            /* Skip duplicates of already chosen terms */
            bool used = false;
            for (size_t k = 0; k < seq_len; ++k) {
                if (seq[k] == cand) {
                    used = true;
                    break;
                }
            }
            if (used)
                continue;

            /* Save current spiral state (will restore on failure) */
            int ox = cur_x, oy = cur_y, od = dir, os = step, om = rem, oc = dir_changes;

            /* Obtain digits of the candidate (little‑endian) */
            uint8_t dig_buf[64];
            size_t dlen = 0;
            get_digits(cand, dig_buf, &dlen);

            /* Storage for positions of digits placed during this trial */
            int pos_x[64], pos_y[64];
            size_t placed = 0;
            bool fail = false;

            /* Place each digit in order (most‑significant first) */
            for (size_t di = 0; di < dlen && !fail; ++di) {
                int digit = dig_buf[dlen - 1 - di];   /* reverse to MSB first */

                /* Move to next cell in the spiral */
                cur_x += dir_dx[dir];
                cur_y += dir_dy[dir];
                --rem;
                if (rem == 0) {
                    dir = (dir + 1) & 3;
                    ++dir_changes;
                    if ((dir_changes & 1) == 0)
                        ++step;
                    rem = step;
                }

                int gx = cur_x + OFFSET;
                int gy = cur_y + OFFSET;

                /* Check the 8 neighbours */
                for (int ddx = -1; ddx <= 1 && !fail; ++ddx) {
                    for (int ddy = -1; ddy <= 1 && !fail; ++ddy) {
                        if (ddx == 0 && ddy == 0)
                            continue;
                        int nx = gx + ddx;
                        int ny = gy + ddy;
                        if (nx < 0 || ny < 0 || nx >= GRID_SIZE || ny >= GRID_SIZE)
                            continue;
                        unsigned char nbr = grid[ny][nx];
                        if (nbr != 255) {
                            int s = digit + nbr;
                            if (isPrime[s])
                                fail = true;
                        }
                    }
                }

                if (fail) {
                    /* Undo any digits placed in this trial */
                    for (size_t p = 0; p < placed; ++p) {
                        int rx = pos_x[p] + OFFSET;
                        int ry = pos_y[p] + OFFSET;
                        grid[ry][rx] = 255;
                    }
                    /* Restore traversal parameters */
                    cur_x = ox;
                    cur_y = oy;
                    dir   = od;
                    step  = os;
                    rem   = om;
                    dir_changes = oc;
                    break;   /* try next candidate integer */
                } else {
                    /* Commit this digit */
                    grid[gy][gx] = (unsigned char)digit;
                    pos_x[placed] = cur_x;
                    pos_y[placed] = cur_y;
                    ++placed;
                }
            }

            /* Success: store the term and break the candidate loop */
            if (!fail) {
                if (seq_len + 1 > seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = cand;
                break;   /* proceed to next element */
            }
            /* otherwise, try the next candidate integer */
        }
    }

    return seq[n - 1];
}
