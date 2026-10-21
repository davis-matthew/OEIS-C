/*
 * A242114  Triangle read by rows: T(n,k) = number of pairs (x,y) in
 *          {1..n}X{1..n} with gcd(x,y) = k.
 * https://oeis.org/A242114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 * Σ_{i=1}^{⌊N/K⌋} φ(i) – 1, where N is the smallest
 *            integer with N(N+1)/2 ≥ n and K = n – N(N–1)/2 (1‑based column
 *            in row N)
 * Range:     u8 n <= 210, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Stored totient values and their prefix sums (pref[i] = Σ_{j=1}^i φ(j)). */
static uint64_t *phi = NULL;          /* φ(i) as 64‑bit unsigned (fits all needed values). */
static seq_t  *pref = NULL;          /* prefix sums in the sequence type. */
static size_t limit = 0;              /* maximum index currently pre‑computed. */

/* Ensure φ and pref are available up to `max_n` (inclusive). */
static void ensure_upto(size_t max_n)
{
    if (max_n <= limit) return;       /* already computed. */

    /* Allocate new arrays. */
    uint64_t *new_phi = (uint64_t*)malloc((max_n + 1) * sizeof(uint64_t));
    seq_t   *new_pref = (seq_t*)   malloc((max_n + 1) * sizeof(seq_t));
    if (!new_phi || !new_pref) return;  /* allocation failed – keep old state. */

    /* Initialize φ[i] = i. */
    for (size_t i = 0; i <= max_n; ++i) new_phi[i] = (uint64_t)i;
    if (max_n >= 0) new_phi[0] = 0;
    if (max_n >= 1) new_phi[1] = 1;

    /* Euler's product sieve for totients. */
    for (size_t p = 2; p <= max_n; ++p) {
        if (new_phi[p] == p) {                 /* p is prime */
            for (size_t j = p; j <= max_n; j += p)
                new_phi[j] -= new_phi[j] / p;
        }
    }

    /* Prefix sums of φ. */
    new_pref[0] = (seq_t)0;
    for (size_t i = 1; i <= max_n; ++i)
        new_pref[i] = new_pref[i-1] + (seq_t)new_phi[i];

    /* Replace old arrays. */
    free(phi);
    free(pref);
    phi = new_phi;
    pref = new_pref;
    limit = max_n;
}

seq_t a(idx_t n)
{
    /* Undefined for n=0, return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Find the row N such that N(N+1)/2 >= n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n) high = mid;
        else low = mid + 1;
    }
    idx_t N = low;

    /* Column K in that row. */
    unsigned __int128 prev = (unsigned __int128)(N - 1) * N / 2;
    idx_t K = n - (idx_t)prev;          /* K ∈ [1,N] */

    /* Compute m = floor(N / K). */
    idx_t m = N / K;                    /* m ≥ 1 */

    /* Ensure prefix sums are available up to m. */
    ensure_upto((size_t)m);

    /* a(n) = 2 * pref[m] – 1. */
    seq_t result = (seq_t)2 * pref[(size_t)m] - (seq_t)1;
    return result;
}
