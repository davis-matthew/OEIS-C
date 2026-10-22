/*
 * A392181  Walk along the square spiral in the first quadrant (see link).
 *          Start with no knights. If the square you are at is not attacked by
 *          an existing knight, place a knight there, otherwise leave it
 *          empty. The sequence lists the squares where the knights are.
 * https://oeis.org/A392181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 9467, u32 n <= 9467, u64 n <= 9467,
 *            u128 n <= 9467, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: scan squares in the first‑quadrant spiral; place a knight
   if it is not attacked by a previously placed knight.  The sequence
   a(n) is the square index of the n‑th knight. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Board size – large enough for n up to about 10 000 in the first quadrant. */
#define BOARD_SIZE 4000

/* Board – 0 = empty, 1 = occupied by a knight. */
static uint8_t board[BOARD_SIZE][BOARD_SIZE] = {0};

/* Dynamic array of knight square indices. */
static seq_t *knight_idx = NULL;
static size_t knight_cnt  = 0;
static size_t knight_cap  = 0;

/* Next square of the spiral to test.  Indexed from 0. */
static idx_t next_sq = 0;

/* ----------------------------------------------------------------------------- */
/* Integer square root, floor(sqrt(n)).  Uses binary search; no floating point. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;      /* hi > sqrt(2^64) */
    idx_t r = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { r = 0; lo = 1; continue; }
        if (mid <= n / mid) { r = mid; lo = mid + 1; }
        else { hi = mid - 1; }
    }
    return r;
}

/* ----------------------------------------------------------------------------- */
/* Coordinates (x,y) of square k in the first‑quadrant spiral.  We use 0‑based
   indices: A319290(k) gives x, A319289(k) gives y. */
static void coord_of_square(idx_t k, idx_t *x, idx_t *y)
{
    if (k == 0) {
        *x = *y = 0;
        return;
    }
    idx_t L = isqrt_u64(k);          /* floor(sqrt(k)) */
    idx_t offset = k - L * L;        /* 0 <= offset < 2*L+1 */

    if (L & 1) {          /* L odd */
        if (offset <= L) {
            *x = L;
            *y = offset;
        } else {
            idx_t j = offset - L - 1;
            *x = L - 1 - j;
            *y = L;
        }
    } else {              /* L even */
        if (offset <= L) {
            *x = offset;
            *y = L;
        } else {
            idx_t j = offset - L - 1;
            *x = L;
            *y = L - 1 - j;
        }
    }
}

/* ----------------------------------------------------------------------------- */
/* Ensure that knight_idx has room for at least 'need' elements. */
static void ensure_knight_cap(size_t need)
{
    if (knight_cap >= need) return;
    size_t new_cap = knight_cap ? knight_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(knight_idx, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory – terminate */
    knight_idx = tmp;
    knight_cap = new_cap;
}

/* ----------------------------------------------------------------------------- */
/* Process the next square of the spiral: if it is unattacked, place a knight
   there and record the square index. */
static void process_next_square(void)
{
    idx_t k = next_sq++;
    idx_t x, y;
    coord_of_square(k, &x, &y);

    /* Knight attack pattern: (±1,±2) and (±2,±1). */
    static const int8_t dx[8] = { 1,  1, -1, -1,  2,  2, -2, -2 };
    static const int8_t dy[8] = { 2, -2,  2, -2,  1, -1,  1, -1 };

    bool attacked = false;
    for (int i = 0; i < 8 && !attacked; ++i) {
        int64_t nx = (int64_t)x + dx[i];
        int64_t ny = (int64_t)y + dy[i];
        if (nx >= 0 && ny >= 0 &&
            nx < BOARD_SIZE && ny < BOARD_SIZE &&
            board[(size_t)nx][(size_t)ny])
            attacked = true;
    }

    if (!attacked) {
        /* Place the knight if within board bounds. */
        if (x < BOARD_SIZE && y < BOARD_SIZE)
            board[(size_t)x][(size_t)y] = 1;

        /* Store the square index. */
        ensure_knight_cap(knight_cnt + 1);
        knight_idx[knight_cnt++] = (seq_t)k;
    }
}

/* ----------------------------------------------------------------------------- */
/* Public sequence function.  a(1) = 0.  Safe for arbitrary call order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1, but harmless */

    /* Generate knights until we have at least n of them. */
    while (knight_cnt < n)
        process_next_square();

    return knight_idx[n - 1];
}
