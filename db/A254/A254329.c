/*
 * A254329  Numbers n such that all x^2 mod n are prime powers (including 0
 *          and 1).
 * https://oeis.org/A254329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Numbers n such that all x^2 mod n are prime powers (including 0 and 1).
 * The sequence a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=6, a(7)=7, a(8)=8, a(9)=9,
 * a(10)=11, a(11)=12, a(12)=14, a(13)=16, a(14)=20, a(15)=32, …
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Prime test for uint64_t */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Prime‑power test (0 and 1 are accepted) */
static bool is_prime_power_uint64(uint64_t v) {
    if (v == 0 || v == 1) return true;
    if (is_prime_uint64(v)) return true;
    uint64_t p = 0;
    if (v % 2 == 0) p = 2;
    else if (v % 3 == 0) p = 3;
    else {
        for (uint64_t i = 5; i * i <= v; i += 6) {
            if (v % i == 0) { p = i; break; }
            if (v % (i + 2) == 0) { p = i + 2; break; }
        }
    }
    if (p == 0) return false;            /* should not occur for composite v */
    uint64_t y = v;
    while (y % p == 0) y /= p;
    return y == 1;
}

/* Check whether m satisfies the OEIS condition */
static bool good_modulus(uint64_t m, uint8_t *visited, size_t cap) {
    if (cap < m) return false;           /* buffer too small, shouldn't happen */
    memset(visited, 0, m);
    for (uint64_t x = 0; x < m; ++x) {
        uint64_t r = (x * x) % m;
        if (visited[r]) continue;
        visited[r] = 1;
        if (r == 0 || r == 1) continue;
        if (!is_prime_power_uint64(r)) return false;
    }
    return true;
}

/* ======= Global cache for generated terms ======= */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t max_checked = 0;

/* Buffer reused for residue tests */
static uint8_t *visited_buf = NULL;
static size_t visited_cap = 0;

/* Public function required by the build */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1; index 0 is undefined */

    /* Generate terms until we reach index n */
    while (seq_len < (size_t)n) {
        ++max_checked;               /* next integer to test */

        /* Resize the visited buffer if necessary */
        if ((size_t)max_checked > visited_cap) {
            uint8_t *tmp = (uint8_t *)realloc(visited_buf, (size_t)max_checked);
            if (!tmp) exit(1);
            visited_buf = tmp;
            visited_cap = (size_t)max_checked;
        }

        if (good_modulus((uint64_t)max_checked, visited_buf, visited_cap)) {
            /* Store the good modulus */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_arr = tmp;
                seq_cap = new_cap;
            }
            seq_arr[seq_len++] = (seq_t)max_checked;
        }
    }
    return seq_arr[n - 1];
}
