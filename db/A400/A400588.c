/*
 * A400588  Numbers k such that A400531(k) = -1; k such that chi_{k,-1}(-1) =
 *          -1, where chi_{k,-1} is the Dirichlet character modulo k with
 *          label -1 in the Conrey labeling system.
 * https://oeis.org/A400588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 9959, u32 n <= 9959, u64 n <= 9959,
 *            u128 n <= 9959, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A400588: numbers k such that
 * if k ≡ 0 (mod 4) then the number of distinct prime factors of k
 * that are ≡ 3 (mod 4) is even,
 * otherwise it is odd.  The sequence consists of all such k in
 * increasing order (offset 1).  The function computes the
 * required term by straightforward enumeration and trial
 * division prime factorisation. */

#include <stdbool.h>
#include <stdint.h>

/* helper: check whether a number satisfies the parity condition */
static bool satisfies(seq_t x)
{
    /* Work only with the 64‑bit part – the test data fits in 64 bits. */
    uint64_t n = (uint64_t)x;

    /* Determine the wanted parity of distinct primes ≡ 3 (mod 4). */
    bool divisible4 = ((x & (seq_t)3) == (seq_t)0);
    int desiredParity = divisible4 ? 0 : 1;
    int parity = 0;                   /* parity of counted primes */

    /* Remove 2’s – they don’t influence the count. */
    if (n % 2 == 0) {
        while (n % 2 == 0)
            n /= 2;
    }

    /* Trial division for remaining odd factors. */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            if (p % 4 == 3)
                parity ^= 1;           /* toggle parity on distinct prime */
            while (n % p == 0)
                n /= p;
        }
    }

    /* If a prime factor > sqrt(n) remains. */
    if (n > 1 && n % 4 == 3)
        parity ^= 1;

    return parity == desiredParity;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n = 0 */

    seq_t k = (seq_t)1;
    idx_t count = 0;

    while (1) {
        if (satisfies(k)) {
            ++count;
            if (count == n)
                return k;
        }
        k++;    /* advance to the next integer */
    }
}
