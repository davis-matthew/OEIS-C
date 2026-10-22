/*
 * A308412  Indices of Gaussian primes on a square spiral.
 * https://oeis.org/A308412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 9404, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A308412: indices k such that A174344(k)+i*A274923(k) is a Gaussian prime.
   The function iterates over the counter‑clockwise square spiral, testing each coordinate for Gaussian primality
   using a simple integer primality test. */

#include <stdint.h>
#include <stdbool.h>

/* Simple trial division prime test for unsigned 64‑bit integers. */
static inline bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    if (n % 3ULL == 0ULL) return n == 3ULL;
    for (uint64_t d = 5ULL; d * d <= n; d += 6ULL) {
        if (n % d == 0ULL || n % (d + 2ULL) == 0ULL) return false;
    }
    return true;
}

/* Test whether the Gaussian integer (x, y) is prime. */
static inline bool is_gaussian_prime(int x, int y) {
    if (x == 0 && y == 0) return false;

    /* Axis: |value| must be prime and ≡ 3 (mod 4). */
    if (x == 0 || y == 0) {
        uint64_t v = (x == 0) ? ((int64_t)llabs((int64_t)y)) : ((int64_t)llabs((int64_t)x));
        return is_prime_u64(v) && (v & 3ULL) == 3ULL;
    }

    /* Non‑axis: norm must be a prime integer. */
    uint64_t ux = (uint64_t)((x >= 0) ? (int64_t)x : -(int64_t)x);
    uint64_t uy = (uint64_t)((y >= 0) ? (int64_t)y : -(int64_t)y);
    uint64_t norm = ux * ux + uy * uy;
    return is_prime_u64(norm);
}

/* Return the nth term of OEIS sequence A308412 (1‑based). */
seq_t a(idx_t n) {
    idx_t count = 0;        /* number of Gaussian primes found so far */
    idx_t k = 1;            /* current index along the spiral (1‑based) */

    int x = 0, y = 0;       /* current coordinate on the spiral */
    int stepLen = 1;        /* number of steps to take in current direction */
    int dir = 0;            /* 0:right, 1:up, 2:left, 3:down */
    int stepsTaken = 0;     /* steps taken in the current direction */
    int turns = 0;          /* number of turns made (increments when direction changes) */

    while (1) {
        if (is_gaussian_prime(x, y)) {
            count++;
            if (count == n) return (seq_t)k;
        }

        /* Move one step in the current direction. */
        switch (dir) {
            case 0: x++; break;
            case 1: y++; break;
            case 2: x--; break;
            case 3: y--; break;
        }
        k++;
        stepsTaken++;

        if (stepsTaken == stepLen) {
            stepsTaken = 0;
            dir = (dir + 1) & 3;
            turns++;
            if (turns % 2 == 0) stepLen++;  /* increase step length after every two turns */
        }
    }
}
