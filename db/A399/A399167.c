/*
 * A399167  Squares visited by a "Leopard" (Knight + Fers + Alfil) moving on a
 *          spirally numbered board always to the lowest available unvisited
 *          square.
 * https://oeis.org/A399167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by simulating a “Leopard” (Knight + Ferz +
 *            Alfil) moving on a spirally numbered board, always taking the
 *            lowest numbered unvisited reachable square
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   spiral_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/spiral_num.h"

/* Dynamic storage for the sequence and its coordinates. */
static seq_t *seq = NULL;
static long long *x = NULL;
static long long *y = NULL;
static size_t seq_cap = 0;   /* allocated capacity */
static size_t seq_len = 0;   /* number of initialized elements */

/* Bit‑set of visited board numbers. */
static unsigned char *visited_bits = NULL;
static size_t visited_bits_bytes = 0;   /* allocated bytes */

/* Ensure visited_bits can hold at least num+1 bits. */
static void ensure_visited_capacity(seq_t num) {
    size_t needed_bytes = ((size_t)num >> 3) + 1;
    if (needed_bytes > visited_bits_bytes) {
        size_t new_bytes = visited_bits_bytes ? visited_bits_bytes << 1 : 16;
        if (new_bytes < needed_bytes) new_bytes = needed_bytes;
        unsigned char *new_bits = (unsigned char*)realloc(visited_bits, new_bytes);
        if (!new_bits) exit(EXIT_FAILURE);
        /* Zero the new region. */
        size_t added = new_bytes - visited_bits_bytes;
        if (added)
            memset(new_bits + visited_bits_bytes, 0, added);
        visited_bits = new_bits;
        visited_bits_bytes = new_bytes;
    }
}

/* Test whether a board number has already been visited. */
static bool visited_is_set(seq_t num) {
    size_t idx = (size_t)num >> 3;
    if (idx >= visited_bits_bytes) return false;
    unsigned char mask = 1 << ((size_t)num & 7);
    return (visited_bits[idx] & mask) != 0;
}

/* Mark a board number as visited. */
static void visited_set(seq_t num) {
    ensure_visited_capacity(num);
    size_t idx = (size_t)num >> 3;
    unsigned char mask = 1 << ((size_t)num & 7);
    visited_bits[idx] |= mask;
}

/* Ensure that seq, x, y have space for at least n elements. */
static void ensure_seq_capacity(size_t n) {
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 256;
    while (new_cap < n) new_cap <<= 1;
    seq_t *new_seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    long long *new_x = (long long*)realloc(x, new_cap * sizeof(long long));
    long long *new_y = (long long*)realloc(y, new_cap * sizeof(long long));
    if (!new_seq || !new_x || !new_y) exit(EXIT_FAILURE);
    seq = new_seq;
    x = new_x;
    y = new_y;
    seq_cap = new_cap;
}

/* One‑time initialisation of the sequence storage. */
static void init_sequence(void) {
    seq_cap = 256;
    seq_len = 1;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    x = (long long*)malloc(seq_cap * sizeof(long long));
    y = (long long*)malloc(seq_cap * sizeof(long long));
    visited_bits_bytes = 16;                     /* 128 bits */
    visited_bits = (unsigned char*)calloc(visited_bits_bytes, 1);
    seq[0] = (seq_t)1;
    x[0] = 0;
    y[0] = 0;
    visited_set((seq_t)1);
}

/* Main function requested by the OEIS build system. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* n<1 is irrelevant for the build */
    if (seq == NULL) init_sequence();

    while (seq_len < n) {
        long long cur_x = x[seq_len - 1];
        long long cur_y = y[seq_len - 1];

        seq_t best_num = 0;
        long long best_x = 0, best_y = 0;
        bool has_best = false;

        /* Knight moves */
        static const int dx_k[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
        static const int dy_k[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
        for (int i = 0; i < 8; ++i) {
            long long nx = cur_x + dx_k[i];
            long long ny = cur_y + dy_k[i];
            seq_t num = spiral_num(nx, ny);
            if (!visited_is_set(num) && (!has_best || num < best_num)) {
                best_num = num;
                best_x = nx;
                best_y = ny;
                has_best = true;
            }
        }

        /* Ferz moves */
        static const int dx_f[4] = { 1, 1, -1, -1 };
        static const int dy_f[4] = { 1, -1, 1, -1 };
        for (int i = 0; i < 4; ++i) {
            long long nx = cur_x + dx_f[i];
            long long ny = cur_y + dy_f[i];
            seq_t num = spiral_num(nx, ny);
            if (!visited_is_set(num) && (!has_best || num < best_num)) {
                best_num = num;
                best_x = nx;
                best_y = ny;
                has_best = true;
            }
        }

        /* Alfil moves */
        static const int dx_a[4] = { 2, 2, -2, -2 };
        static const int dy_a[4] = { 2, -2, 2, -2 };
        for (int i = 0; i < 4; ++i) {
            long long nx = cur_x + dx_a[i];
            long long ny = cur_y + dy_a[i];
            seq_t num = spiral_num(nx, ny);
            if (!visited_is_set(num) && (!has_best || num < best_num)) {
                best_num = num;
                best_x = nx;
                best_y = ny;
                has_best = true;
            }
        }

        if (!has_best) return 0;   /* should never happen */

        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = best_num;
        x[seq_len] = best_x;
        y[seq_len] = best_y;
        visited_set(best_num);
        seq_len++;
    }
    return seq[n - 1];
}
