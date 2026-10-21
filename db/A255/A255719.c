/*
 * A255719  Number of length 4 1..(n+2) arrays with no leading partial sum
 *          equal to a prime and no consecutive values equal.
 * https://oeis.org/A255719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of length‑4 arrays (x1,x2,x3,x4) with
 *   1 ≤ xi ≤ n+2,
 *   xi ≠ xi+1 for i = 1..3,
 *   no partial sum S_k = ∑_{i=1}^k xi (k = 1..4) is prime.
 *
 * The program counts all quadruples by fixing (x1,x2) and
 * precomputing for every possible sum S2 = x1+x2 the number
 * of admissible (x3,x4) pairs; it then subtracts the pairs with
 * x3 = x2.  All calculations are performed with integer arithmetic
 * only, using the provided integer types.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    idx_t L = n + 2;                  /* maximum value of each entry   */
    idx_t maxSum = 4 * L;             /* upper bound for any partial sum */

    /* ---------- Prime sieve ------------------------------------- */
    bool *prime = (bool *)malloc((size_t)(maxSum + 1) * sizeof(bool));
    if (!prime) return 0;             /* out of memory */
    for (size_t i = 0; i <= maxSum; ++i) prime[i] = true;
    prime[0] = prime[1] = false;
    for (size_t p = 2; p * p <= (size_t)maxSum; ++p) {
        if (prime[p]) {
            for (size_t q = p * p; q <= (size_t)maxSum; q += p)
                prime[q] = false;
        }
    }

    /* ---------- Pref. prime counts (for < 3L window) ----------- */
    idx_t *primePrefix =
        (idx_t *)malloc((size_t)(maxSum + 1) * sizeof(idx_t));
    idx_t cnt = 0;
    for (size_t i = 0; i <= (size_t)maxSum; ++i) {
        if (prime[i]) ++cnt;
        primePrefix[i] = cnt;
    }

    /* ---------- Precompute number of valid d for each S3 ---------- */
    idx_t maxS3 = 3 * L;
    seq_t *compCountAdd = (seq_t *)malloc((size_t)(maxS3 + 1) * sizeof(seq_t));
    for (idx_t s3 = 1; s3 <= maxS3; ++s3) {
        idx_t end = s3 + L;
        if (end > maxSum) end = maxSum;
        idx_t primesInInterval = primePrefix[end] - primePrefix[s3];
        compCountAdd[s3] = (seq_t)L - (seq_t)primesInInterval;
    }

    /* ---------- Precompute total pairs for each S2 ---------------- */
    idx_t maxS2 = 2 * L;
    seq_t *totalPairsS2 = (seq_t *)malloc((size_t)(maxS2 + 1) * sizeof(seq_t));
    for (idx_t s2 = 2; s2 <= maxS2; ++s2) {
        seq_t total = 0;
        for (idx_t c = 1; c <= L; ++c) {
            idx_t s3 = s2 + c;
            if (prime[s3]) continue;           /* S3 must be not prime */
            seq_t add = compCountAdd[s3];
            if (!prime[s2 + 2 * c]) add -= 1;    /* exclude d == c */
            total += add;
        }
        totalPairsS2[s2] = total;
    }

    /* ---------- Main counting ------------------------------------ */
    seq_t ans = 0;
    for (idx_t a = 1; a <= L; ++a) {
        if (prime[a]) continue;               /* S1 cannot be prime */
        for (idx_t b = 1; b <= L; ++b) {
            if (a == b) continue;             /* consecutive equal */
            idx_t s2 = a + b;
            if (prime[s2]) continue;          /* S2 cannot be prime */

            seq_t subtotal = totalPairsS2[s2];

            /* subtract the cases where x3 == b (invalid) */
            idx_t s3b = s2 + b;
            if (!prime[s3b]) {
                seq_t sub = compCountAdd[s3b];
                if (!prime[s2 + 2 * b])     /* exclude d == b when d counted */
                    sub -= 1;
                subtotal -= sub;
            }

            ans += subtotal;
        }
    }

    /* ---------- Clean up ---------------------------------------- */
    free(prime);
    free(primePrefix);
    free(compCountAdd);
    free(totalPairsS2);

    return ans;
}
