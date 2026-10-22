/*
 * A376318  The number of distinct values of x+y+z (mod n) when x*y*z = 1 (mod
 *          n).
 * https://oeis.org/A376318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9590, u32 n <= 9590, u64 n <= 9590,
 *            u128 n <= 9590, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS:A376318  –  The number of distinct values of (x+y+z) (mod n)
 *  when x·y·z ≡ 1 (mod n).  
 *  Algorithm:
 *      1. Enumerate all units modulo n, i.e. all x with gcd(x,n)=1;
 *      2. For each unit store its modular inverse inv[x] (mod n);
 *      3. For every ordered pair (x,y) of units compute
 *            z = inv[x]*inv[y] (mod n)
 *         and record the sum s = (x + y + z) (mod n);
 *      4. Count the distinct sums recorded.
 *  Complexity:  O(φ(n)²) with φ the Euler totient; works fine for n≤10000.
 *  (No closed formula is known.)
 *
 *  The function obeys all the interface and implementation constraints
 *  specified in the problem statement.                       */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Extended Euclidean algorithm for modular inverse (mod<n).  */
static uint64_t mod_inv_uint64(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a;
    while (newr != 0) {
        uint64_t q = (uint64_t)(r / newr);
        int64_t tmp = newt;
        newt = t - (int64_t)q * newt;
        t = tmp;
        tmp = newr;
        newr = r - (int64_t)q * newr;
        r = tmp;
    }
    /* r == 1 for coprime a and mod */
    if (t < 0) t += (int64_t)mod;
    return (uint64_t)t;
}

/* Minimal dynamic buffers shared across calls */
static unsigned char *seen = NULL;      /* reset for each n */
static size_t seen_cap = 0;
static uint64_t *units = NULL;          /* list of units */
static uint64_t *inv_units = NULL;      /* their inverses */
static size_t units_cap = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined but harmless */
    if (n == 1) return (seq_t)1;       /* by definition */

    /* Ensure seen array fits n */
    if (n > seen_cap) {
        free(seen);
        seen = (unsigned char *)malloc((size_t)n);
        seen_cap = n;
    }
    /* Ensure unit buffers fit at most n entries */
    if (n > units_cap) {
        free(units);
        free(inv_units);
        units    = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
        inv_units= (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
        units_cap = n;
    }

    /* 1. Collect units and their inverses */
    size_t phi = 0;
    for (uint64_t i = 1; i < n; ++i) {
        /* gcd test */
        uint64_t a = i, b = n;
        while (b) { uint64_t t = a % b; a = b; b = t; }
        if (a == 1) {
            units[phi]     = i;
            inv_units[phi] = mod_inv_uint64(i, n);
            ++phi;
        }
    }

    /* 2. Reset seen markers */
    memset(seen, 0, (size_t)n);

    /* 3. Enumerate ordered pairs of units */
    for (size_t ix = 0; ix < phi; ++ix) {
        uint64_t x     = units[ix];
        uint64_t invx  = inv_units[ix];
        for (size_t iy = 0; iy < phi; ++iy) {
            uint64_t y    = units[iy];
            uint64_t invy = inv_units[iy];
            /* compute z = invx * invy (mod n) using 128‑bit to avoid overflow */
            uint64_t z = (uint64_t)(((unsigned __int128)invx * invy) % n);
            uint64_t s = (x + y + z) % n;
            seen[s] = 1;
        }
    }

    /* 4. Count distinct sums */
    uint64_t cnt = 0;
    for (size_t s = 0; s < n; ++s) {
        if (seen[s]) ++cnt;
    }

    return (seq_t)cnt;
}
