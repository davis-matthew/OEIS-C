/*
 * A389543  a(n) is the largest value t such that the sequence defined by
 *          b(t+2) = (b(t+1) * b(t)) mod n with b(1) = b(2) = 2 mod n where
 *          all pairs of terms {b(t-1),b(t)} are distinct.
 * https://oeis.org/A389543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the largest t such that all pairs {b(t-1),b(t)} are
 *            distinct * where b(1)=b(2)=2 (mod n) and b(t+2) = (b(t+1)*b(t))
 *            mod n. * This is obtained as a(n) = μ + λ + 1, where μ is the
 *            pre‑period * length and λ is the period length of the pair
 *            sequence * (b(t),b(t+1))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef struct { idx_t a, b; } pair_t;

/* Advance one step of the pair sequence:
 * given current pair (x,y) -> next pair (y, (x*y)%mod) */
static inline pair_t next_pair(pair_t cur, idx_t mod) {
    /* Use 128‑bit to avoid overflow of x*y (for 64‑bit idx_t) */
    unsigned __int128 mul = (unsigned __int128)cur.a * cur.b;
    idx_t nxt = (idx_t)(mul % mod);
    pair_t res = { cur.b, nxt };
    return res;
}

/* Main function required by the build */
seq_t a(idx_t n) {
    /* Base pair (b1,b2) where b1=b2=2 mod n */
    pair_t start = { 2 % n, 2 % n };

    /* Floyd cycle detection */
    pair_t tortoise = next_pair(start, n);
    pair_t hare = next_pair(next_pair(start, n), n);

    /* Find first meeting point */
    while (tortoise.a != hare.a || tortoise.b != hare.b) {
        tortoise = next_pair(tortoise, n);
        hare = next_pair(next_pair(hare, n), n);
    }

    /* Find μ (pre‑period length) */
    idx_t mu = 0;
    tortoise = start;
    while (tortoise.a != hare.a || tortoise.b != hare.b) {
        tortoise = next_pair(tortoise, n);
        hare = next_pair(hare, n);
        ++mu;
    }

    /* Find λ (period length) */
    idx_t lam = 1;
    hare = next_pair(tortoise, n);
    while (tortoise.a != hare.a || tortoise.b != hare.b) {
        hare = next_pair(hare, n);
        ++lam;
    }

    /* a(n) = μ + λ + 1 (number of terms before a pair repeats) */
    seq_t result = (seq_t)mu + (seq_t)lam + (seq_t)1;
    return result;
}
