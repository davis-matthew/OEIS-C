/*
 * A316885  Squares visited by queen moves on a spirally numbered board,
 *          always moving to the lowest available unvisited prime square at
 *          each step.
 * https://oeis.org/A316885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by simulating a queen walk on the square‑spiral board
 *            always moving to the lowest available unvisited prime square at
 *            each step
 * Range:     u8 n <= 50, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h> /* for abs */

/* board limit – sufficient for n up to at least 10000 */
static const idx_t BOARD_LIMIT = 2000000U;

/* static data structures */
static int16_t *board_x;
static int16_t *board_y;
static uint32_t *visit_stamp;
static uint32_t *prime_list;
static size_t prime_cnt;
static uint32_t global_mark = 1;
static bool init_done = false;

/* ---- initialization ---------------------------------------------------- */
static void init_data(void)
{
    /* allocate coordinate arrays */
    board_x = (int16_t *)malloc((BOARD_LIMIT + 1) * sizeof(int16_t));
    board_y = (int16_t *)malloc((BOARD_LIMIT + 1) * sizeof(int16_t));
    visit_stamp = (uint32_t *)calloc((BOARD_LIMIT + 1), sizeof(uint32_t));

    /* generate spiral coordinates */
    int16_t x = 0, y = 0;
    board_x[1] = x;
    board_y[1] = y;
    idx_t cur = 1;
    idx_t step = 1;
    while (cur < BOARD_LIMIT) {
        for (idx_t i = 0; i < step && cur < BOARD_LIMIT; ++i) {
            ++x;
            ++cur;
            board_x[cur] = x;
            board_y[cur] = y;
        }
        for (idx_t i = 0; i < step && cur < BOARD_LIMIT; ++i) {
            ++y;
            ++cur;
            board_x[cur] = x;
            board_y[cur] = y;
        }
        ++step;
        for (idx_t i = 0; i < step && cur < BOARD_LIMIT; ++i) {
            --x;
            ++cur;
            board_x[cur] = x;
            board_y[cur] = y;
        }
        for (idx_t i = 0; i < step && cur < BOARD_LIMIT; ++i) {
            --y;
            ++cur;
            board_x[cur] = x;
            board_y[cur] = y;
        }
        ++step;
    }

    /* sieve primes up to BOARD_LIMIT */
    bool *is_prime = (bool *)malloc((BOARD_LIMIT + 1) * sizeof(bool));
    for (idx_t i = 0; i <= BOARD_LIMIT; ++i) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; (idx_t)i * i <= BOARD_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= BOARD_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* build prime list */
    prime_cnt = 0;
    for (idx_t i = 2; i <= BOARD_LIMIT; ++i) {
        if (is_prime[i]) ++prime_cnt;
    }
    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    size_t p_idx = 0;
    for (idx_t i = 2; i <= BOARD_LIMIT; ++i) {
        if (is_prime[i]) {
            prime_list[p_idx++] = (uint32_t)i;
        }
    }

    free(is_prime);
    init_done = true;
}

/* ---- sequence ----------------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) init_data();

    if (n == 1u) return (seq_t)1;

    /* advance global mark (handle overflow) */
    uint32_t mark = ++global_mark;
    if (global_mark == 0) {                     /* overflowed */
        memset(visit_stamp, 0, (BOARD_LIMIT + 1) * sizeof(uint32_t));
        global_mark = 1;
        mark = global_mark++;
    }

    visit_stamp[1] = mark;
    uint32_t cur = 1;
    int16_t cx = board_x[cur];
    int16_t cy = board_y[cur];

    for (idx_t step = 2; step <= n; ++step) {
        uint32_t next = 0;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint32_t p = prime_list[pi];
            if (visit_stamp[p] == mark) continue;       /* already visited in this run */

            int16_t px = board_x[p];
            int16_t py = board_y[p];
            int dx = (int)px - (int)cx;
            int dy = (int)py - (int)cy;
            if (dx == 0 || dy == 0 || abs(dx) == abs(dy)) {
                next = p;
                break;
            }
        }
        /* should always find a reachable prime; but guard just in case */
        if (next == 0) return (seq_t)cur;

        visit_stamp[next] = mark;
        cur = next;
        cx = board_x[cur];
        cy = board_y[cur];
    }
    return (seq_t)cur;
}
