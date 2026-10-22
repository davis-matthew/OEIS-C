/*
 * A376509  Natural numbers whose iterated squaring modulo 100 eventually
 *          enters the 4-cycle 21, 41, 81, 61.
 * https://oeis.org/A376509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A376509 – Natural numbers whose iterated squaring modulo 100
 *  eventually enters the 4‑cycle 21, 41, 81, 61.
 *
 *  The set of residues modulo 100 that belong to this basin is
 *  periodic with period 100.  If S[0..p-1] are these residues in
 *  increasing order, then for n ≥ 1
 *
 *      a(n) = 100 * ⌊(n-1)/p⌋ + S[(n-1) mod p]
 *
 *  This function computes the nth term using that property.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
  static bool initialized = false;
  static seq_t S[100];
  static idx_t p;          /* number of residues in the basin */

  if (!initialized) {
    int f[100];
    for (int r = 0; r < 100; ++r) f[r] = (r * r) % 100;

    /* residues that belong to the desired 4‑cycle */
    bool in_basin[100] = {false};
    in_basin[21] = in_basin[41] = in_basin[81] = in_basin[61] = true;

    /* propagate pre‑images until a fixed point is reached */
    bool changed;
    do {
      changed = false;
      for (int r = 0; r < 100; ++r) {
        if (!in_basin[r] && in_basin[f[r]]) {
          in_basin[r] = true;
          changed = true;
        }
      }
    } while (changed);

    /* collect sorted residues belonging to the basin */
    for (int r = 0; r < 100; ++r)
      if (in_basin[r])
        S[p++] = (seq_t)r;

    initialized = true;
  }

  idx_t block  = (n - 1) / p;
  idx_t offset = (n - 1) % p;
  seq_t term   = (seq_t)block * 100 + S[offset];
  return term;
}
