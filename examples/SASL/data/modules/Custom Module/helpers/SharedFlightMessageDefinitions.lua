-- properties.lua
-- © 2017 Lionel Zamouth

-- Helpers for manipulating properties - paradise of the lazy ones
-- Many of these assume properties used for logics are either 0 or 1.

dProp = defineProperty
cProp = createProperty
--~ function defp(prop,default) defineProperty(prop,default) end

-- shortcuts for assigning 0 or 1 or 6 to a property.(6 = failure)
function set0(prop) set(prop,0) end
function set1(prop) set(prop,1) end
function set2(prop) set(prop,2) end
function set3(prop) set(prop,3) end
function set4(prop) set(prop,4) end
function set5(prop) set(prop,5) end
function set6(prop) set(prop,6) end
-- alias
zero = set0
fail = set6
-- when dealing with boolean properties (NOT datarefs)
function setTrue(prop) set(prop,true)  end
function setFalse(prop) set(prop,false) end
-- alias
setF=setFalse
setT=setTrue

-- return bool, where anything but 0 is true. Warning: nil is expected to throw an error.
-- if not dealing with a propery, use i2b(val) instead.
function bool(prop) return not (get(prop) == 0) end

-- set property to 1 or 0, depending on condition.
--~ function setbool(prop,cond) if cond then set(prop,1) else set(prop,0) end end
function setbool(prop,cond) set(prop, cond and 1 or 0) end
function setfail(prop,cond) set(prop, cond and 6 or 0) end
-- alias
setBool = setbool

-- return bool for 1 or 0 explicitely. get1() is NOT same as bool(), get0() is (not bool())
function is0(prop)	return get(prop) == 0 end
function is1(prop)	return get(prop) == 1 end
function is2(prop)	return get(prop) == 2 end
function is6(prop)	return get(prop) == 6 end
function isgz(prop)	return get(prop) >  0 end
function islz(prop)	return get(prop) <  0 end
function isnz(prop)	return not is0(prop)  end
isfail = is6
--~ function isT(prop)	return get(prop) end
--~ function isF(prop)	return not get(prop) end

-- switches value of property between 0 and 1.
function flip(prop) set(prop, 1-get(prop)) end
-- switches value of property between false and true.
function notp(prop) set(prop, not get(prop)) end

-- copy property to another one
function copy(prop1, prop2) set(prop2, get(prop1)) end

-- increment/decrement property
function inc(prop) set(prop, get(prop) + 1) end
function dec(prop) set(prop, get(prop) - 1) end
