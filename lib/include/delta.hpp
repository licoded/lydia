#pragma once
/*
 * This file is part of Lydia.
 *
 * Lydia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Lydia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Lydia.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "cuddObj.hh"
#include "logger.hpp"
#include "propositional_logic.hpp"
#include "visitor.hpp"
#include <logic.hpp>
#include <utility>

namespace whitemech {
namespace lydia {

/*!
 * This visitor implements the 'delta' function of [1].
 *
 * [1] Brafman, Ronen I., Giuseppe De Giacomo, and Fabio Patrizi.
 *     "LTLf/LDLf non-markovian rewards."
 *     Thirty-Second AAAI Conference on Artificial Intelligence. 2018.
 */
class DeltaVisitor : public Visitor {
private:
protected:
  std::shared_ptr<const PropositionalFormula> result;
  const set_atoms_ptr prop_interpretation;
  bool epsilon;

public:
  static Logger logger;
  DeltaVisitor() : epsilon{true} {}
  explicit DeltaVisitor(const set_atoms_ptr &prop_interpretation)
      : DeltaVisitor(prop_interpretation, false) {}
  explicit DeltaVisitor(set_atoms_ptr prop_interpretation, bool epsilon)
      : prop_interpretation{std::move(prop_interpretation)}, epsilon{epsilon} {}

  // callbacks for LDLf
  void visit(const Symbol &) override{};
  void visit(const LDLfBooleanAtom &) override;
  void visit(const LDLfAnd &) override;
  void visit(const LDLfOr &) override;
  void visit(const LDLfNot &) override;
  void visit(const LDLfDiamond &) override;
  void visit(const LDLfBox &) override;

  // callbacks for regular expressions
  void visit(const PropositionalRegExp &) override{};
  void visit(const TestRegExp &) override{};
  void visit(const UnionRegExp &) override{};
  void visit(const SequenceRegExp &) override{};
  void visit(const StarRegExp &) override{};

  // callbacks for propositional logic
  void visit(const PropositionalTrue &) override{};
  void visit(const PropositionalFalse &) override{};
  void visit(const PropositionalAtom &) override{};
  void visit(const PropositionalAnd &) override{};
  void visit(const PropositionalOr &) override{};
  void visit(const PropositionalNot &) override{};

  void visit(const QuotedFormula &) override{};

  std::shared_ptr<const PropositionalFormula> apply(const LDLfFormula &b);
};

class DeltaDiamondRegExpVisitor : Visitor {
protected:
  std::shared_ptr<const PropositionalFormula> result;
  const LDLfTemporal &formula;
  const set_atoms_ptr prop_interpretation;
  bool epsilon;

public:
  explicit DeltaDiamondRegExpVisitor(set_atoms_ptr prop_interpretation,
                                     const LDLfTemporal &formula, bool epsilon)
      : prop_interpretation{std::move(prop_interpretation)},
        formula{std::move(formula)}, epsilon{epsilon} {}
  explicit DeltaDiamondRegExpVisitor(const LDLfTemporal &formula, bool epsilon)
      : formula{std::move(formula)}, epsilon{epsilon} {};

  // callbacks for regular expressions
  void visit(const PropositionalRegExp &) override;
  void visit(const TestRegExp &) override;
  void visit(const UnionRegExp &) override;
  void visit(const SequenceRegExp &) override;
  void visit(const StarRegExp &) override;

  std::shared_ptr<const PropositionalFormula> apply(const RegExp &b);
};

class DeltaBoxRegExpVisitor : Visitor {
protected:
  std::shared_ptr<const PropositionalFormula> result;
  const LDLfTemporal &formula;
  const set_atoms_ptr prop_interpretation;
  bool epsilon;

public:
  explicit DeltaBoxRegExpVisitor(set_atoms_ptr prop_interpretation,
                                 const LDLfTemporal &formula, bool epsilon)
      : prop_interpretation{std::move(prop_interpretation)},
        formula{std::move(formula)}, epsilon{epsilon} {}
  explicit DeltaBoxRegExpVisitor(const LDLfTemporal &formula, bool epsilon)
      : formula{std::move(formula)}, epsilon{epsilon} {};
  // callbacks for regular expressions
  void visit(const PropositionalRegExp &) override;
  void visit(const TestRegExp &) override;
  void visit(const UnionRegExp &) override;
  void visit(const SequenceRegExp &) override;
  void visit(const StarRegExp &) override;
  std::shared_ptr<const PropositionalFormula> apply(const RegExp &b);
};

/*!
 * Apply the DeltaVisitor (with epsilon = true).
 *
 * * @return the propositional logic formula with atoms as quoted LDLf formulas.
 *        | In the case epsilon is true, always return either True or False.
 */
/*TODO the last bit implies doing some simplification of the Ands/Ors, or
 * doing some post-processing (e.g. evaluate the formula with a "dummy model").
 */
std::shared_ptr<const PropositionalFormula> delta(const LDLfFormula &);

/*!
 * Apply the DeltaVisitor (with epsilon = false).
 *
 * @return the propositional logic formula.
 */
std::shared_ptr<const PropositionalFormula> delta(const LDLfFormula &,
                                                  const set_atoms_ptr &);

} // namespace lydia
} // namespace whitemech