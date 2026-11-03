/*
 * A383183  Square spiral numbers of the n-th grid point visited by a king
 *          always moving to the unvisited point labeled with the smallest
 *          possible prime or else composite number.
 * https://oeis.org/A383183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 171, u32 n <= 171, u64 n <= 171,
 *            u128 n <= 171, big any n
 * Validated: full, pass, 172 terms through n = 171 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A383183 – Square spiral numbers visited by a king
   Algorithm:
   • The grid is labeled by the square spiral of A316328
     (0 at (0,0), 1 on the point (1,0), 2 on (1,1), 3 on (0,1), …).
   • At each step the king chooses among unvisited neighbours the point
     with the smallest prime label; if none, the smallest composite label.
   • The function a(n) simulates this walk from the origin up to index n.
   The implementation uses a hash table for visited coordinates and
   a compact coordinate→label mapping. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------------- Spiral coordinate → label ----------------- */
static inline seq_t coord_to_label(int64_t x, int64_t y)
{
    if (x == 0 && y == 0) return 0;
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t r = ax > ay ? ax : ay;                 /* ring */
    seq_t start = (seq_t)(2 * r - 1) * (seq_t)(2 * r - 1);     /* first label of ring */

    if (x == r && y >= -r + 1) {                  /* right side upward */
        seq_t offset = (seq_t)(y + r - 1);
        return start + offset;
    }
    if (y == r && x <= r - 1 && x >= -r) {        /* top side leftward */
        seq_t offset = (seq_t)(2 * r + (r - 1 - x));
        return start + offset;
    }
    if (x == -r && y <= r - 1 && y >= -r) {       /* left side downward */
        seq_t offset = (seq_t)(4 * r + (r - 1 - y));
        return start + offset;
    }
    /* bottom side rightward */
    seq_t offset = (seq_t)(6 * r + (x + r - 1));
    return start + offset;
}

/* ----------------- Hash table for visited coordinates ----------------- */
#define HASH_BITS 15
#define HASH_SIZE (1U << HASH_BITS)
#define HASH_MASK (HASH_SIZE - 1)

static uint64_t vt_keys[HASH_SIZE];
static unsigned char vt_used[HASH_SIZE];

static inline uint64_t hash_uint64(uint64_t x)
{
    /* splitmix64 hash */
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049b133111ebdULL;
    return x ^ (x >> 31);
}

static inline size_t hash_idx(uint64_t key)
{
    return hash_uint64(key) & HASH_MASK;
}

static inline size_t hash_find(uint64_t key)
{
    size_t i = hash_idx(key);
    while (vt_used[i]) {
        if (vt_keys[i] == key) return i;
        i = (i + 1) & HASH_MASK;
    }
    return HASH_SIZE;
}

static inline void hash_insert(uint64_t key)
{
    size_t i = hash_idx(key);
    while (vt_used[i]) {
        if (vt_keys[i] == key) return;
        i = (i + 1) & HASH_MASK;
    }
    vt_keys[i] = key;
    vt_used[i] = 1;
}

/* ----------------- Main function ----------------- */
static const int neigh[8][2] = {
    { 1,  0}, { 1,  1}, { 0,  1}, { -1,  1},
    {-1,  0}, {-1, -1}, { 0, -1}, { 1, -1}
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* reset visited set for each call */
    memset(vt_used, 0, sizeof(vt_used));

    int64_t curx = 0, cury = 0;
    seq_t curLabel = 0;
    hash_insert(((uint64_t)(uint32_t)curx << 32) | (uint32_t)cury);

    for (idx_t step = 1; step <= n; ++step) {
        seq_t bestPrime = 0, bestOther = 0;
        int64_t bestPrimeX = 0, bestPrimeY = 0;
        int64_t bestOtherX = 0, bestOtherY = 0;

        for (int k = 0; k < 8; ++k) {
            int64_t tx = curx + neigh[k][0];
            int64_t ty = cury + neigh[k][1];
            uint64_t key = ((uint64_t)(uint32_t)tx << 32) | (uint32_t)ty;
            if (hash_find(key) != HASH_SIZE) continue;   /* already visited */

            seq_t lbl = coord_to_label(tx, ty);
            if (is_prime_big(lbl)) {
                if (bestPrime == 0 || lbl < bestPrime) {
                    bestPrime = lbl;
                    bestPrimeX = tx;
                    bestPrimeY = ty;
                }
            } else {
                if (bestPrime == 0 && (bestOther == 0 || lbl < bestOther)) {
                    bestOther = lbl;
                    bestOtherX = tx;
                    bestOtherY = ty;
                }
            }
        }

        if (bestPrime != 0) {
            curLabel = bestPrime;
            curx = bestPrimeX;
            cury = bestPrimeY;
        } else {
            curLabel = bestOther;
            curx = bestOtherX;
            cury = bestOtherY;
        }
        hash_insert(((uint64_t)(uint32_t)curx << 32) | (uint32_t)cury);

        if (step == n) return curLabel;
    }
    return curLabel;  /* should never reach here */
}
